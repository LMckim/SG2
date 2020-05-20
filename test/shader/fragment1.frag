#version 120
// #version 430
uniform sampler2D colourMap;
uniform sampler2D normalMap;

//light properties
uniform vec2 mPos;
uniform vec3 lightColour = vec3(1.0);
uniform float lightPower = 0.1;
uniform vec4 lightSpecColour = vec4(0.4, 0.4, 0.4, 1.0);
uniform float lightSpecPower = 0.08;

uniform vec2 reflectOffset;
uniform vec2 resolution = vec2(1024.0, 1024.0);
uniform bool invertBumpY = true;

void main()
{
    vec3 lightPosition = vec3(vec2(mPos.x, (mPos.y )),0.01);

    vec2 coord = gl_TexCoord[0].xy;
    vec4 diffuseColour = texture2D(colourMap, coord);
    vec3 normalColour = texture2D(normalMap, coord).rgb;

    //get normal value from sample
    normalColour.g = invertBumpY ? 1.0 - normalColour.g : normalColour.g;
    vec3 normal = normalize(normalColour * 2.0 - 1.0);

    //calculate the light vector
    vec3 lightDir = vec3((lightPosition.xy - gl_FragCoord.xy) / resolution, lightPosition.z);
        
    //calculate the colour intensity based on normal and add specular to pixel facing light
    float colourIntensity = max(dot(normal, normalize(lightDir)), 0.0);
    vec4 specular = vec4(0.0);
    vec4 diffuse = vec4(0.0);

    // if(colourIntensity > 0.0)
    // {
    //     //vector half way between light and view direction
    //     vec3 halfVec = normalize(lightDir + vec3(0.5, 0.5, 0.5)); //fixed 2D view, so view is centred
    //     //get specular value from map
    //     vec4 specColour = vec4(texture2D(specularMap, gl_TexCoord[0].xy).rgb, 1.0);
    //     float specModifier = max(dot(normal, halfVec), 0.0);
    //     specular = pow(specModifier, lightSpecPower) * specColour * lightSpecColour;
    //     specular.a *= diffuseColour.a;
    //     diffuse = lightSpecColour * diffuseColour * colourIntensity;        
    // }

    diffuseColour.rgb += ((lightColour * lightPower) * colourIntensity);
    diffuseColour.rgb *= diffuseColour.rgb;

    gl_FragColor = clamp(specular + diffuse + diffuseColour, 0.0, 1.0);
}