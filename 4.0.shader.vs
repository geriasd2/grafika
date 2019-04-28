#version 400 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 FragPos;

// ezt most magabol a vertex adatbol tudjuk de szamolhatnank a szomszedos vertex csucsokbol is (cross product)
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    // a model matrixal transzformaljuk a fragmenteket es ezt adjuk tovabb a fs nek, mert minden fenyviszony szamitast most a world spacben szamolunk

    FragPos = vec3(model * vec4(aPos, 1.0)); // kell h a light directiont tudjuk szamolni az fs ben


    /// a norma vektor a local spacben van de mivel a szalitasokat a world spacben vegezzuk ezert transzformalnunk kell azt
    /// azonban a n vektorok nem jelolnek poziciot es nincs homogen reszuk, tehat ha szorozzuk a model matrixal a n vektort nem szabad hasznalni a transzlacios komponenset csak a felso 3x3 matrixat azaz csak rotalni es scalelni akarjuk

    // transpose of the inverse uper 3x3 model matris is the normal matrix
    // a non uniform scaling miatt es nem kell uf=gye atranslation komponens

    Normal = mat3(transpose(inverse(model))) * aNormal;

    //Normal = aNormal;

    gl_Position = projection * view * vec4(FragPos, 1.0);
}
