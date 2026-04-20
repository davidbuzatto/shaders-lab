/**
 * @file GameWorld.h
 * @author Prof. Dr. David Buzatto
 * @brief GameWorld implementation.
 * 
 * @copyright Copyright (c) 2026
 */
#include <stdio.h>
#include <stdlib.h>

#include "GameWorld.h"
#include "ResourceManager.h"

#include "raylib/raylib.h"
#include "raylib/raymath.h"
//#define RAYGUI_IMPLEMENTATION    // to use raygui, comment these three lines.
//#include "raylib/raygui.h"       // other compilation units must only include
//#undef RAYGUI_IMPLEMENTATION     // raygui.h

#define trace( ... ) TraceLog( LOG_INFO, __VA_ARGS__ );

#define MIN_REAL -2.0
#define MAX_REAL 0.47
#define MIN_IMG -1.12
#define MAX_IMG 1.11

static Shader hsvShader;
static Shader mandelbrotShader;
static Shader kishimisuTutorialShader;

static int hueAdjust = 0;

static float minReal = MIN_REAL;
static float maxReal = MAX_REAL;
static float minImg = MIN_IMG;
static float maxImg = MAX_IMG;

static int clickX;
static int clickY;
static float zoomCenterReal;
static float zoomCenterImg;
static float timeToZoom = 0.016f;
static float timeToZoomCounter = 0.0f;
static bool startAutomaticZoom = false;

static void zoomIn( void );
static void hsvShaderTest( void );
static void mandelbrotShaderTest( void );
static void kishimisuTutorialShaderTest( void );

/**
 * @brief Creates a dinamically allocated GameWorld struct instance.
 */
GameWorld *createGameWorld( void ) {

    GameWorld *gw = (GameWorld*) malloc( sizeof( GameWorld ) );
    gw->dummy = 0;

    hsvShader = LoadShader( NULL, "resources/shaders/hsvShader.fs" );
    mandelbrotShader = LoadShader( NULL, "resources/shaders/mandelbrotShader.fs" );
    kishimisuTutorialShader = LoadShader( NULL, "resources/shaders/kishimisuTutorialShader.fs" );

    return gw;

}

/**
 * @brief Destroys a GameWindow object and its dependecies.
 */
void destroyGameWorld( GameWorld *gw ) {
    UnloadShader( hsvShader );
    UnloadShader( mandelbrotShader );
    free( gw );
}

/**
 * @brief Reads user input and updates the state of the game.
 */
void updateGameWorld( GameWorld *gw, float delta ) {

    if ( IsKeyDown( KEY_UP ) ) {
        hueAdjust++;
    }

    if ( IsKeyDown( KEY_DOWN ) ) {
        hueAdjust--;
    }

    if ( IsKeyPressed( KEY_R ) ) {
        minReal = MIN_REAL;
        maxReal = MAX_REAL;
        minImg = MIN_IMG;
        maxImg = MAX_IMG;
        startAutomaticZoom = false;
    }

    /*if ( IsMouseButtonPressed( MOUSE_BUTTON_LEFT ) ) {
        zoomIn();
    }*/

    if ( IsMouseButtonPressed( MOUSE_BUTTON_LEFT ) ) {
        clickX = GetMouseX();
        clickY = GetMouseY();
        zoomCenterReal = Lerp( minReal, maxReal, (float) clickX / GetScreenWidth() );
        zoomCenterImg = Lerp( minImg, maxImg, (float) clickY / GetScreenWidth() );
        startAutomaticZoom = true;
    }

    if ( startAutomaticZoom ) {

        //zoomCenterReal = Lerp( minReal, maxReal, (float) GetMouseX() / GetScreenWidth() );
        //zoomCenterImg = Lerp( minImg, maxImg, (float) GetMouseY() / GetScreenWidth() );

        timeToZoomCounter += delta;
        if ( timeToZoomCounter > timeToZoom ) {
            timeToZoomCounter = 0.0f;
            zoomIn();
        }

    }

}

/**
 * @brief Draws the state of the game.
 */
void drawGameWorld( GameWorld *gw ) {

    BeginDrawing();
    ClearBackground( BLACK );

    //hsvShaderTest();
    //mandelbrotShaderTest();
    kishimisuTutorialShaderTest();

    EndDrawing();

}

void zoomIn( void ) {

    float w = maxReal - minReal;
    float h = maxImg - minImg;

    w *= 0.995f;
    h *= 0.995f;

    minReal = zoomCenterReal - w / 2.0f;
    maxReal = zoomCenterReal + w / 2.0f;
    minImg = zoomCenterImg - w / 2.0f;
    maxImg = zoomCenterImg + w / 2.0f;

}

void hsvShaderTest( void ) {

    float mouseX = GetMouseX();
    float mouseY = GetMouseY();
    float screenWidth = GetScreenWidth();
    float screenHeight = GetScreenHeight();

    int mouseXLoc = GetShaderLocation( hsvShader, "mouseX" );
    int mouseYLoc = GetShaderLocation( hsvShader, "mouseY" );
    int screenWidthLoc = GetShaderLocation( hsvShader, "screenWidth" );
    int screenHeightLoc = GetShaderLocation( hsvShader, "screenHeight" );

    SetShaderValue( hsvShader, mouseXLoc, &mouseX, SHADER_UNIFORM_FLOAT );
    SetShaderValue( hsvShader, mouseYLoc, &mouseY, SHADER_UNIFORM_FLOAT );
    SetShaderValue( hsvShader, screenWidthLoc, &screenWidth, SHADER_UNIFORM_FLOAT );
    SetShaderValue( hsvShader, screenHeightLoc, &screenHeight, SHADER_UNIFORM_FLOAT );

    int w = 50;

    for ( int i = 0; i < 10; i++ ) {
        BeginShaderMode( hsvShader );
        DrawRectangle( 10 + w * i / 2, 10 + w * i / 2, w, w, WHITE );
        EndShaderMode();
        DrawRectangleLines( 10 + w * i / 2, 10 + w * i / 2, w, w, BLACK );
    }

}

void mandelbrotShaderTest( void ) {

    float mouseX = GetMouseX();
    float mouseY = GetMouseY();
    float screenWidth = GetScreenWidth();
    float screenHeight = GetScreenHeight();

    int mouseXLoc = GetShaderLocation( mandelbrotShader, "mouseX" );
    int mouseYLoc = GetShaderLocation( mandelbrotShader, "mouseY" );
    int screenWidthLoc = GetShaderLocation( mandelbrotShader, "screenWidth" );
    int screenHeightLoc = GetShaderLocation( mandelbrotShader, "screenHeight" );
    int hueAdjustLoc = GetShaderLocation( mandelbrotShader, "hueAdjust" );

    int minRealLoc = GetShaderLocation( mandelbrotShader, "minReal" );
    int maxRealLoc = GetShaderLocation( mandelbrotShader, "maxReal" );
    int minImgLoc = GetShaderLocation( mandelbrotShader, "minImg" );
    int maxImgLoc = GetShaderLocation( mandelbrotShader, "maxImg" );

    SetShaderValue( mandelbrotShader, mouseXLoc, &mouseX, SHADER_UNIFORM_FLOAT );
    SetShaderValue( mandelbrotShader, mouseYLoc, &mouseY, SHADER_UNIFORM_FLOAT );
    SetShaderValue( mandelbrotShader, screenWidthLoc, &screenWidth, SHADER_UNIFORM_FLOAT );
    SetShaderValue( mandelbrotShader, screenHeightLoc, &screenHeight, SHADER_UNIFORM_FLOAT );
    SetShaderValue( mandelbrotShader, hueAdjustLoc, &hueAdjust, SHADER_UNIFORM_INT );

    SetShaderValue( mandelbrotShader, minRealLoc, &minReal, SHADER_UNIFORM_FLOAT );
    SetShaderValue( mandelbrotShader, maxRealLoc, &maxReal, SHADER_UNIFORM_FLOAT );
    SetShaderValue( mandelbrotShader, minImgLoc, &minImg, SHADER_UNIFORM_FLOAT );
    SetShaderValue( mandelbrotShader, maxImgLoc, &maxImg, SHADER_UNIFORM_FLOAT );

    BeginShaderMode( mandelbrotShader );
    DrawRectangle( 0, 0, GetScreenWidth(), GetScreenHeight(), WHITE );
    EndShaderMode();

}

void kishimisuTutorialShaderTest( void ) {

    float mouseX = GetMouseX();
    float mouseY = GetMouseY();
    float screenWidth = GetScreenWidth();
    float screenHeight = GetScreenHeight();

    int mouseXLoc = GetShaderLocation( kishimisuTutorialShader, "mouseX" );
    int mouseYLoc = GetShaderLocation( kishimisuTutorialShader, "mouseY" );
    int screenWidthLoc = GetShaderLocation( kishimisuTutorialShader, "screenWidth" );
    int screenHeightLoc = GetShaderLocation( kishimisuTutorialShader, "screenHeight" );
    int timeLoc = GetShaderLocation( kishimisuTutorialShader, "time" );

    float time = (float) GetTime();

    SetShaderValue( kishimisuTutorialShader, mouseXLoc, &mouseX, SHADER_UNIFORM_FLOAT );
    SetShaderValue( kishimisuTutorialShader, mouseYLoc, &mouseY, SHADER_UNIFORM_FLOAT );
    SetShaderValue( kishimisuTutorialShader, screenWidthLoc, &screenWidth, SHADER_UNIFORM_FLOAT );
    SetShaderValue( kishimisuTutorialShader, screenHeightLoc, &screenHeight, SHADER_UNIFORM_FLOAT );
    SetShaderValue( kishimisuTutorialShader, timeLoc, &time, SHADER_UNIFORM_FLOAT );

    BeginShaderMode( kishimisuTutorialShader );
    DrawRectangle( 0, 0, GetScreenWidth(), GetScreenHeight(), WHITE );
    EndShaderMode();

}