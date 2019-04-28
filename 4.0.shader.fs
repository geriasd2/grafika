#version 400 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos; // kell h tudjuk szamitani a light directiont

uniform vec3 lightPos; // mivel ez csak egy single pozicio es nem valtozik legyen uniform
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

uniform float a;
uniform float d;
uniform float s;

void main() {
    // a valo vilagban altalaban nem pontszeru fenyforras van, hanem nagyon sok fenyforras szorodik korolottunk, meg a mas objektumokrol visszavert feny is hatassal van a megvilagitasra, ez a globalis illumination model
    // ennek a komplexitasat elkerulve hasznaljuk az ambient modelt ami egy konstans szint ad az objektumokhoz meg akkor is ha nincs direkt fenyforras hiszen a valo vilagban egy kis szorodas mindig van

    // ambient
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * lightColor;

    //////////////////////////////////////////////////////

    /// Normal vector: a vector that is perpendicular to the vertex' surface.
    //  The directed light ray: a direction vector that is the difference vector between the light's position and the fragment's position. To calculate this light ray we need the light's position vector and the fragment's position vector.

    // diffuse
    vec3 norm = normalize(Normal);
    // feny irany kiszamitasa (normalizalni kell)
    vec3 lightDir = normalize(lightPos - FragPos);

    // a dot productnal fontos h mind a ket vektor normalizalt legyen
    // ha a szog 90 foknal nagyobb akkor negativ lenne a dot product ezert van a max
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    //////////////////////////////////////////////////////

    // specular
    /// itt a normal vektor a fenyirany es a nezo iranytol is fugg a latvany
    // fugg az anyag tukrozo kepessegetol, mennyitre veri vissza a fenyt
    // minel kisebb a szog a visszavert es a nezo irany kozott annal nagyobb a specular feny hatasa
    // a kamera object pozicioja lesz a nezo pozicioja a world scene ben ebbol es a fragment poziciobol kiszamoljuk a view directiont

    //  specular intensity value to give the specular highlight a medium-bright color so that it doesn't have too much of an impact
    // kiprobal 1 el is erosebb lesz
    float specularStrength = 0.5;

    vec3 viewDir = normalize(viewPos - FragPos);

    // lightDir = normalize(lightPos - FragPos);
    // a reflect fuggveny viszont azt varja h a fenyforrasbol mutasson a fragmentre ezert negaljuk
    vec3 reflectDir = reflect(-lightDir, norm);

    // a szog a view es a reflect kozott
    // 32 value is the shininess value of the highlight.
    //he higher the shininess value of an object, the more it properly reflects the light instead of scattering it all around and thus the smaller the highlight becomes.
    // 32, 64 ... 256
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    //////////////////////////////////////////////////////

    //vec3 result = ambient * objectColor;
    //vec3 result = diffuse * objectColor;
    //vec3 result = specular * objectColor;
    //vec3 result = (ambient + specular) * objectColor;

    vec3 tmp= vec3(0,0,0);
    if(a == 1) {
        tmp += ambient;
    }
    if(d == 1) {
        tmp += diffuse;
    }
    if(s == 1) {
        tmp += specular;
    }
    vec3 result = tmp * objectColor;
    FragColor = vec4(result, 1.0);
}
