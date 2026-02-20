#version 330

// coordenada da textura
in vec2 fragTexCoord;

// cor atual
in vec4 fragColor;

// cor de retorno
out vec4 finalColor;

// uniforms (são imutáveis)
uniform float mouseX;
uniform float mouseY;
uniform float screenWidth;
uniform float screenHeight;

vec3 colorFromHSV( float hue, float saturation, float value );
void testeCores();

void main() {
    testeCores();
}

void testeCores() {

    float x = gl_FragCoord.x / screenWidth;
    float y = gl_FragCoord.y / screenHeight;

    float mX = mouseX / screenWidth;
    float mY = mouseY / screenHeight;

    finalColor = vec4( colorFromHSV( mX * 360, 1.0, 1.0 ), 1 );

}

vec3 colorFromHSV( float hue, float saturation, float value ) {
    
    vec3 color = vec3( 0, 0, 0 );

    float k = mod( 5.0 + hue / 60.0, 6.0 );
    float t = 4.0 - k;
    k = (t < k) ? t : k;
    k = clamp( k, 0.0, 1.0 );
    color.r = value - value * saturation * k;

    k = mod( 3.0 + hue / 60.0, 6.0 );
    t = 4.0 - k;
    k = (t < k) ? t : k;
    k = clamp( k, 0.0, 1.0 );
    color.g = value - value * saturation * k;

    k = mod( 1.0f + hue / 60.0, 6.0 );
    t = 4.0 - k;
    k = (t < k) ? t : k;
    k = clamp( k, 0.0, 1.0 );
    color.b = value - value * saturation * k;

    return color;
    
}