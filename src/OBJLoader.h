#pragma once
#include "Mesh.h"

namespace Utilities {
    class OBJLoader {
    public:
        // Constructor
        OBJLoader();
        ~OBJLoader();

        // Functions
        bool LoadObj(Data::Mesh *mesh);
    };
}