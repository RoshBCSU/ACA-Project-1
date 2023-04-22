#define BHT_SIZE 16
#define PHT_SIZE 16
#define PHT_COUNTER_SIZE 2

// Branch History Table
unsigned char bht[BHT_SIZE] = {0};

// Pattern History Table
unsigned char pht[PHT_SIZE] = {0};

// Branch Target Buffer
typedef struct {
    unsigned int tag;
    unsigned int target;
} btb_entry;

btb_entry btb[PHT_SIZE];
int btb_head = 0;

// Helper function to get the index of the PHT
unsigned int get_pht_index(unsigned int pc) {
    unsigned int mask = (1 << PHT_COUNTER_SIZE) - 1;
    return (bht[pc % BHT_SIZE] ^ (pc >> 2)) & mask;
}

// GShare predictor function
bool predict(unsigned int pc) {
    // Get index into BHT and PHT
    unsigned int bht_index = pc % BHT_SIZE;
    unsigned int pht_index = get_pht_index(pc);

    // Get prediction from PHT
    unsigned char pht_counter = pht[pht_index];

    // Predict taken if PHT counter is 2 or 3, not taken otherwise
    bool predicted_taken = (pht_counter >= 2);

    // Check if there is a BTB entry for this branch
    if (predicted_taken) {
        unsigned int tag = pc >> 2;
        for (int i = 0; i < PHT_SIZE; i++) {
            if (btb[i].tag == tag) {
                // BTB hit: return target address
                return true;
            }
        }
        // BTB miss: fetch from PC+1
        return false;
    }

    return predicted_taken;
}

// Helper function to update the PHT
void update_pht(unsigned int pc, bool taken) {
    // Get index into BHT and PHT
    unsigned int bht_index = pc % BHT_SIZE;
    unsigned int pht_index = get_pht_index(pc);

    // Update PHT counter based on actual outcome
    if (taken) {
        if (pht[pht_index] < 3) {
            pht[pht_index]++;
        }
    } else {
        if (pht[pht_index] > 0) {
            pht[pht_index]--;
        }
    }

    // Update BHT based on actual outcome
    bht[bht_index] = ((bht[bht_index] << 1) | taken) & ((1 << PHT_COUNTER_SIZE) - 1);
}

// Helper function to update the BTB
void update_btb(unsigned int pc, unsigned int target) {
    // Create new BTB entry
    btb_entry new_entry;
    new_entry.tag = pc >> 2;
    new_entry.target = target;

    // Check if there is already an entry with this tag
    for (int i = 0; i < PHT_SIZE; i++) {
        if (btb[i].tag == new_entry.tag) {
            // Replace existing entry with new one
            btb[i] = new_entry;
            return;
        }
    }

    // Add new entry to BTB (using FIFO replacement)
    btb[btb_head] = new_entry;
    btb_head = (btb_head + 1) % PHT_SIZE;
}

// GShare branch resolution function
void resolve(unsigned int pc, bool taken, unsigned int target) {
    // Update PHT and BHT based on actual outcome
    update_pht(pc, taken);
    // Update BTB if branch is taken
    if (taken) {
        update_btb(pc, target);
    }
}

int main() {
unsigned int pc = 0;
bool taken = false;
unsigned int target = 0;
pc = 0x1000;
// Predict branch outcome
taken = predict(pc);
// Simulate execute stage
if (taken) {
// Branch is taken
target = 0x2000;
// Simulate memory stage
// Simulate write-back stage
// Resolve branch
resolve(pc, taken, target);
// Fetch next instruction from branch target
pc = target;
} else {
// Branch is not taken
// Simulate memory stage
// Simulate write-back stage
// Resolve branch
resolve(pc, taken, target);
// Fetch next instruction sequentially
pc += 4;
}
// Repeat for next instruction
return 0;
}