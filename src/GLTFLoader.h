#pragma once
#include "Mesh.h"

namespace Utilities {
    class GLTFLoader {
    public:
        // Constructors
        GLTFLoader();
        ~GLTFLoader();

        // Functions
        bool LoadGLTF(Data::Mesh *mesh);
    private:
        // Helper functions
        void PrintErrors(std::string *error, std::string *warning);
    };
}