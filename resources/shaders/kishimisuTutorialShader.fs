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
uniform float time;

vec3 palette( float t );
void mainImage( vec4 fragColor, vec2 fragCoord, vec2 iResolution );

void main() {
    
    vec2 iResolution = vec2( screenWidth, screenHeight );
    vec2 uv = ( gl_FragCoord.xy * 2.0 - iResolution.xy ) / iResolution.y;
    vec2 uv0 = uv;
    vec3 color = vec3( 0.0 );
    
    for ( float i = 0.0; i < 4.0; i++ ) {
    
        uv = fract( uv * 1.5 ) - 0.5;

        float d = length( uv ) * exp( -length( uv ) );
        
        vec3 col = palette( length( uv0 ) + i * .4 + time * .4 );

        d = sin( d * 8. + time ) / 8.;
        d = abs( d );

        d = pow( 0.01 / d, 2.0 );

        color += col * d;
        
    }
    
    finalColor = vec4( color, 1.0 );

}

vec3 palette( float t ) {

    vec3 a = vec3( 0.5, 0.5, 0.5 );
    vec3 b = vec3( 0.5, 0.5, 0.5 );
    vec3 c = vec3( 1.0, 1.0, 1.0 );
    vec3 d = vec3( 0.263, 0.416, 0.557 );
    
    return a + b * cos( 6.28318 * ( c * t * d ) );
    
}