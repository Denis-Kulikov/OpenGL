#pragma once
#define NUM_BONES_PER_VEREX 4

struct VertexBoneData {
    VertexBoneData() {
        std::memset(IDs, 0, sizeof(IDs));
        std::memset(Weights, 0, sizeof(Weights));
    }

    // Добавление кости в первую свободную ячейку
    void AddBoneData(unsigned int BoneID, float Weight) {
        for (unsigned int i = 0 ; i < NUM_BONES_PER_VEREX ; i++) {
            if (Weights[i] == 0.0) {
                IDs[i]     = BoneID;
                Weights[i] = Weight;
                return;
            }
        }
    
        // иначе заменяем минимальный
        // int   minIndex = 0;
        // float minW = Weights[0];
        // for (int i = 1; i < size; ++i) {
        //     if (Weights[i] < minW) { minW = Weights[i]; minIndex = i; }
        // }
        // if (Weight > minW) {
        //     IDs[minIndex]     = (int)BoneID;
        //     Weights[minIndex] = Weight;
        // }
    }

    void NormalizeWeights() {
        float total = 0.0f;
        for (unsigned int i = 0; i < NUM_BONES_PER_VEREX; i++)
            total += Weights[i];

        if (total > 0.0f) {
            for (unsigned int i = 0; i < NUM_BONES_PER_VEREX; i++)
                Weights[i] /= total;
        }
    }

    int IDs[NUM_BONES_PER_VEREX];
    float Weights[NUM_BONES_PER_VEREX];
};