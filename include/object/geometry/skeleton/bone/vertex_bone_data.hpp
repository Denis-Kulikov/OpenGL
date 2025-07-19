#pragma once
#define NUM_BONES_PER_VEREX 4

struct VertexBoneData {
    VertexBoneData() {
        for (unsigned int i = 0 ; i < size ; i++) {
            IDs[i] = 0;
            Weights[i] = 0;
        }
    }

    void AddBoneData(unsigned int BoneID, float Weight) {
        for (unsigned int i = 0 ; i < size ; i++) {
            if (Weights[i] == 0.0) {
                IDs[i]     = BoneID;
                Weights[i] = Weight;
                return;
            }
        }
    }

    void NormalizeWeights() {
        float total = 0.0f;
        for (unsigned int i = 0; i < size; i++)
            total += Weights[i];

        if (total > 0.0f) {
            for (unsigned int i = 0; i < size; i++)
                Weights[i] /= total;
        }
    }

    static const int size = NUM_BONES_PER_VEREX;
    int IDs[size];
    float Weights[size];
};