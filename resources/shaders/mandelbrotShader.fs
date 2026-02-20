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
uniform int hueAdjust;

uniform float minReal;
uniform float maxReal;
uniform float minImg;
uniform float maxImg;

void mandelbrotBest( int maxIterations, int hueAdjust, float minReal, float maxReal, float minImg, float maxImg );
void mandelbrotNaive();
vec3 colorFromHSV( float hue, float saturation, float value );

void main() {
    mandelbrotBest( 2000, hueAdjust, minReal, maxReal, minImg, maxImg );
}

void mandelbrotBest( int maxIterations, int hueAdjust, float minReal, float maxReal, float minImg, float maxImg ) {

    float xS = gl_FragCoord.x / screenWidth;
    float yS = 1.0 - gl_FragCoord.y / screenHeight; // inverte y

    /*float x0 = mix( -2.0, 0.47, xS );
    float y0 = mix( -1.12, 1.11, yS );*/
    float x0 = mix( minReal, maxReal, xS );
    float y0 = mix( minImg, maxImg, yS );

    float x = 0;
    float y = 0;
    float x2 = 0;
    float y2 = 0;

    int iteration = 0;

    while ( x2 + y2 <= 4 && iteration < maxIterations ) {
        x2 = x * x;
        y2 = y * y;
        y = 2 * x * y + y0;
        x = x2 - y2 + x0;
        iteration++;
    }

    //float v = iteration / float(maxIterations);
    //float v = ( iteration % 100 ) / 100.0;
    //float v = ( iteration + 30 ) % 360;
    //finalColor = vec4( colorFromHSV( v, 1.0, 1.0 ), 1.0 );

    float itCalc = 0;

    if ( iteration < maxIterations ) {
        float logZn = log( x * x + y * y ) / 2;
        float nu = log( logZn / log( 2.0 ) ) / log( 2.0 );
        itCalc = iteration + 1 - nu;
    }

    iteration = int( floor( itCalc ) );

    float v1 = ( iteration + hueAdjust ) % 360;
    float v2 = ( iteration + hueAdjust + 1 ) % 360;

    vec4 color1 = vec4( colorFromHSV( v1, 1.0, 1.0 ), 1.0 );
    vec4 color2 = vec4( colorFromHSV( v2, 1.0, 1.0 ), 1.0 );

    if ( iteration == 0 ) {
        color1.rbg = vec3( 0, 0, 0 );
    }

    finalColor = mix( color1, color2, itCalc - iteration );

}

void mandelbrotNaive() {

    float xS = gl_FragCoord.x / screenWidth;
    float yS = gl_FragCoord.y / screenHeight;

    float x0 = mix( -2.0, 0.47, xS );
    float y0 = mix( -1.12, 1.11, yS );

    float x = 0;
    float y = 0;

    int iteration = 0;
    int maxIterations = 1000;

    while ( x * x + y * y < 2 * 2 && iteration < maxIterations ) {
        float xTemp = x * x - y * y + x0;
        y = 2 * x * y + y0;
        x = xTemp;
        iteration++;
    }

    float v = iteration / float(maxIterations);

    finalColor = vec4( v, v, v, 1.0 );

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
