#pragma once
#define NUM_BONES_PER_VEREX 4

struct VertexBoneData {
    void AddBoneData(unsigned int BoneID, float Weight) {
        for (unsigned int i = 0 ; i < size ; i++) {
            if (Weights[i] == 0.0) {
                IDs[i]     = BoneID;
                Weights[i] = Weight;
                return;
            }
        }
    }
    static const int size = NUM_BONES_PER_VEREX;
    unsigned int IDs[size];
    float Weights[size];
};