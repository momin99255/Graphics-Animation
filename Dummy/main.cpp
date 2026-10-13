// filepath: main.cpp

/* * =========================================================================
 * HEADER INCLUDES & PLATFORM COMPATIBILITY
 * =========================================================================
 */
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

//#include <opencv2/opencv.hpp>
#include <stdlib.h>
#include <cmath>
#include <cstdio>
#include <cstring>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/* * =========================================================================
 * EXTERNAL OS-SPECIFIC MULTIMEDIA TRIGGERS
 * =========================================================================
 */
#ifdef _WIN32
    #define VIDEO_FILE "C:\\Users\\YourName\\Videos\\movie.mp4"
    #define OPEN_CMD   "start "
#elif __APPLE__
    #define VIDEO_FILE "/Users/yourname/Movies/movie.mp4"
    #define OPEN_CMD   "open "
#else
    #define VIDEO_FILE "/home/momin/OBS/Endgame.mp4" // Local video path for Linux
    #define OPEN_CMD   "xdg-open "
#endif

/* * =========================================================================
 * GLOBAL INTERACTIVE VIEW CONTROL VARIABLES
 * =========================================================================
 */
float degreeX =  20.0f;
float degreeY = -20.0f;
float degreeZ =   0.0f;

float tx = 0.0f;
float ty = 0.0f;
float tz = -7.0f;
float zoom = 1.0f;

float angle = 0.0f;
double ex = 0, ey = 0, ez = 5;

/* * =========================================================================
 * INTERACTIVE CHAIR RENDERING STATES
 * =========================================================================
 */
static bool  seatFolded  = false;
static float seatFoldAng = 0.0f;
static float ledPulse    = 0.0f;

static void drawEntryGate();
static void drawExitGate();

/* * =========================================================================
 * DYNAMIC MOVIE SCREEN EMISSIVE STATES & OPENCV CAPTURE
 * =========================================================================
 */
static bool  videoPlaying    = false;
static float screenGlow      = 0.0f;
static float screenScanLine  = 0.0f;  // 🟢 Duplication error permanently fixed here!

// OpenCV Global Tracking Objects
//cv::VideoCapture cap;
GLuint videoTextureID = 0;

static bool  entryGateOpen  = false;
static float entryGateAngle = 0.0f;

/* * =========================================================================
 * VIEWPORT RESHAPE CALLBACK
 * =========================================================================
 * Adjusts the aspect ratio and frustum when the window is resized.
 */
static void resize(int width, int height)
{
    if(height <= 0) height = 1;
    const float ar = (float)width / (float)height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

/* * =========================================================================
 * PRIMITIVE GEOMETRY HELPERS
 * =========================================================================
 */

/**
 * @brief Renders a scaled solid sphere to simulate soft materials like cushions.
 */
static void softAt(float x, float y, float z, float w, float h, float d)
{
    glPushMatrix();
    glTranslatef(x, y, z);
    glScalef(w, h, d);
    glutSolidSphere(0.5f, 20, 14);
    glPopMatrix();
}

/**
 * @brief Renders a closed cylinder along the Z-axis.
 */
static void cylZ(float r, float len, int sl)
{
    GLUquadric* q = gluNewQuadric();
    gluQuadricNormals(q, GLU_SMOOTH);
    gluCylinder(q, r, r, len, sl, 1);
    gluDisk(q, 0, r, sl, 1);
    glTranslatef(0,0,len);
    gluDisk(q, 0, r, sl, 1);
    gluDeleteQuadric(q);
}

/**
 * @brief Renders an open cylinder (without bottom cover) along the Z-axis.
 */
static void cylZOpen(float r, float len, int sl)
{
    GLUquadric* q = gluNewQuadric();
    gluQuadricNormals(q, GLU_SMOOTH);
    gluDisk(q, 0, r, sl, 1);
    gluCylinder(q, r, r, len, sl, 1);
    gluDeleteQuadric(q);
}

/**
 * @brief Renders a hollow flat disk (annulus), useful for cup holder rims.
 */
static void annularDisk(float rInner, float rOuter, int sl)
{
    GLUquadric* q = gluNewQuadric();
    gluQuadricNormals(q, GLU_SMOOTH);
    gluDisk(q, rInner, rOuter, sl, 1);
    gluDeleteQuadric(q);
}

/**
 * @brief Renders a cylinder aligned with the Y-axis.
 */
static void cylY(float x, float y, float z, float r, float h, int sl)
{
    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(-90, 1, 0, 0);
    cylZ(r, h, sl);
    glPopMatrix();
}

/* * =========================================================================
 * COLOR PALETTES
 * =========================================================================
 */
static void cOrange()   { glColor3f(0.85f, 0.38f, 0.05f); }
static void cOrangeHi() { glColor3f(1.00f, 0.55f, 0.15f); }
static void cOrangeDk() { glColor3f(0.55f, 0.22f, 0.02f); }
static void cShell()    { glColor3f(0.09f, 0.09f, 0.11f); }
static void cMetal()    { glColor3f(0.18f, 0.18f, 0.20f); }
static void cMetalHi()  { glColor3f(0.40f, 0.40f, 0.42f); }
static void cCarpet()   { glColor3f(0.14f, 0.08f, 0.04f); }
static void cCupRim()   { glColor3f(0.25f, 0.25f, 0.27f); }

/**
 * @brief Renders a basic cube scaled to specified dimensions.
 */
static void box(float w, float h, float d)
{
    glPushMatrix();
    glScalef(w, h, d);
    glutSolidCube(1.0f);
    glPopMatrix();
}

/**
 * @brief Renders a basic cube at a specific 3D coordinate.
 */
static void boxAt(float x, float y, float z, float w, float h, float d)
{
    glPushMatrix();
    glTranslatef(x, y, z);
    box(w, h, d);
    glPopMatrix();
}

/* * =========================================================================
 * THEATRE ENVIRONMENT RENDERING
 * =========================================================================
 */

/**
 * @brief Draws the main theatre floor, including stepped seating platforms
 * and the central walking runway.
 */
static void drawFloor()
{
    const int   ROWS    = 10;
    const float RISE    = 0.38f;
    const float SPACING = 1.22f;
    const float FRONT_Z = 1.20f;
    const float BASE_Y  = -1.36f;
    const float SX_L    = -3.40f;
    const float SX_R    =  13.00f;
    const float W       = SX_R - SX_L;

    for(int r = 0; r < ROWS; ++r)
    {
        float topY   = BASE_Y + (r+1)*RISE;
        float solidH = (r+1)*RISE;
        float stepZ  = FRONT_Z - r*SPACING;
        float centerZ = FRONT_Z - (r + 0.5f) * SPACING;

        glEnable(GL_LIGHTING);
        glColor3f(0.22f, 0.12f, 0.06f);
        boxAt(SX_L, BASE_Y + solidH * 0.5f, centerZ, W, solidH, SPACING);

        const float TOP_THICK = 0.02f;
        const float TILE_W = 0.55f;
        int tiles = (int)std::max(1.0f, std::floor(W / TILE_W));
        float tileDepth = SPACING * 0.98f;
        float startXTile = SX_L - W*0.5f + TILE_W*0.5f;
        float topCenterY = topY - TOP_THICK*0.5f;
        for(int t = 0; t < tiles; ++t)
        {
            float tx = startXTile + t * TILE_W;
            if(((r + t) & 1) == 0) glColor3f(0.24f, 0.14f, 0.07f);
            else                  glColor3f(0.18f, 0.10f, 0.05f);
            boxAt(tx, topCenterY, centerZ, TILE_W * 0.95f, TOP_THICK, tileDepth);
        }
    }

    glEnable(GL_LIGHTING);
    glColor3f(0.20f, 0.11f, 0.05f);
    float runwayCenterX = -11.0f;
    float runwayCenterZ = -1.2f;
    float runwayDepth = FRONT_Z + 1.40f;
    boxAt(runwayCenterX, BASE_Y, runwayCenterZ, 22.0f, 0.04f, runwayDepth);

    const float FLOOR_TILE = 0.60f;
    const float TILE_THK = 0.002f;
    float floorXMin = -11.0f - 0.2f;
    float floorXMax =  11.0f + 0.2f;
    float floorZMin = -13.50f;
    float floorZMax = 18.0f;
    int tilesX = (int)std::max(1.0f, std::floor((floorXMax - floorXMin) / FLOOR_TILE));
    int tilesZ = (int)std::max(1.0f, std::floor((floorZMax - floorZMin) / FLOOR_TILE));
    float startX = floorXMin + FLOOR_TILE*0.5f;
    float startZ = floorZMin + FLOOR_TILE*0.5f;
    float tileY = BASE_Y + 0.01f + TILE_THK*0.5f;
    for(int ix=0; ix<tilesX; ++ix)
    {
        for(int iz=0; iz<tilesZ; ++iz)
        {
            float cx = startX + ix * FLOOR_TILE;
            float cz = startZ + iz * FLOOR_TILE;
            if(((ix+iz)&1)==0) glColor3f(0.22f, 0.12f, 0.06f);
            else               glColor3f(0.18f, 0.09f, 0.05f);
            boxAt(cx, tileY, cz, FLOOR_TILE*0.96f, TILE_THK, FLOOR_TILE*0.96f);
        }
    }
}

/**
 * @brief Draws metallic borders/steps on rows (utility function).
 */
static void drawRowStep(float y, float z, float width, float depth)
{
    cMetal();
    boxAt(0.0f, y, z, width, 0.16f, depth);
}

/**
 * @brief Renders the front wall located directly behind the movie screen.
 */
static void drawFrontWall()
{
    const float WALL_Z    =  20.50f;
    const float WALL_H    =   9.00f;
    const float WALL_BY   =  -1.36f;
    const float WALL_XL   = -11.00f;
    const float WALL_XR   =  11.00f;
    const float WW        = WALL_XR - WALL_XL;
    const float THICK     =   0.20f;

    glEnable(GL_LIGHTING);
    glColor3f(0.0f, 0.0f, 0.0f);
    boxAt(0.0f, WALL_BY + WALL_H * 0.5f, WALL_Z + THICK * 0.5f, WW + 0.4f, WALL_H + 0.2f, THICK);

    const int PANELS = 4;
    const float GAP = 0.12f;
    const float panelW = (WW - (PANELS - 1) * GAP) / PANELS;
    const float panelH = WALL_H - 0.4f;
    const float panelY = WALL_BY + 0.2f + panelH * 0.5f;

    for(int i = 0; i < PANELS; ++i)
    {
        float cx = WALL_XL + panelW * 0.5f + i * (panelW + GAP);
        glColor3f(0.012f, 0.012f, 0.015f);
        boxAt(cx, panelY, WALL_Z, panelW, panelH, 0.02f);
        glColor3f(0.005f, 0.005f, 0.005f);
        boxAt(cx, panelY, WALL_Z + 0.011f, panelW * 0.97f, panelH * 0.97f, 0.01f);
    }
}

/**
 * @brief Draws the side walls populated with animated LED light strips.
 * Calculates exclusions to ensure gates are not covered by lights.
 */
static void drawSideLEDWall(float xSide)
{
    const float WALL_Z_NEA = -13.50f;
    const float WALL_Z_FAR =  20.50f;
    const float MARGIN_Z   = 0.85f;
    const float LED_Z_START = WALL_Z_NEA + MARGIN_Z;
    const float LED_Z_END   = WALL_Z_FAR - MARGIN_Z;
    const float WALL_H      =   9.00f;
    const float BASE_Y      =  -1.36f;

    float midY = BASE_Y + WALL_H * 0.50f;
    float stripOffsets[3] = { -0.30f, 0.0f, 0.30f };

    float pulse = 0.72f + 0.28f * std::sin(ledPulse * 1.8f);
    float proj = (xSide < 0) ? 0.025f : -0.025f;

    const float SEG_LEN = 0.6f;
    const float W_OUT   = 0.02f, H_OUT = 0.08f;
    const float W_CORE  = 0.01f, H_CORE = 0.030f;

    const float ENTRY_GZ = -10.5f, ENTRY_GW = 2.5f;
    const float EXIT_GZ  = 13.10f, EXIT_GW  = 2.25f;
    const float EXCLUDE_PAD = 1.0f;

    for(int s = 0; s < 3; ++s)
    {
        float sy = midY + stripOffsets[s];

        glDisable(GL_LIGHTING);
        glColor3f(1.00f * pulse, 0.80f * pulse, 0.12f * pulse);
        for(float z = LED_Z_START; z < LED_Z_END; z += SEG_LEN)
        {
            float centerZ = z + SEG_LEN*0.5f;
            if(xSide > 0)
            {
                if(centerZ > ENTRY_GZ - ENTRY_GW*0.5f - EXCLUDE_PAD && centerZ < ENTRY_GZ + ENTRY_GW*0.5f + EXCLUDE_PAD)
                    continue;
            }
            else
            {
                if(centerZ > EXIT_GZ - EXIT_GW*0.5f - EXCLUDE_PAD && centerZ < EXIT_GZ + EXIT_GW*0.5f + EXCLUDE_PAD)
                    continue;
            }
            boxAt(xSide + proj - 0.01f, sy - H_OUT*0.5f, centerZ, W_OUT, H_OUT, SEG_LEN);
        }

        glColor3f(1.0f, 0.95f, 0.55f);
        for(float z = LED_Z_START; z < LED_Z_END; z += SEG_LEN)
        {
            float centerZ = z + SEG_LEN*0.5f;
            boxAt(xSide + proj, sy - H_CORE*0.5f, centerZ, W_CORE, H_CORE, SEG_LEN);
        }

        glEnable(GL_LIGHTING);
        glColor3f(1.0f, 0.90f, 0.50f);
        for(float bz = LED_Z_START + 1.5f; bz < LED_Z_END; bz += 2.4f)
        {
            float centerZ = bz;
            if(xSide > 0)
            {
                if(centerZ > ENTRY_GZ - ENTRY_GW*0.5f - EXCLUDE_PAD && centerZ < ENTRY_GZ + ENTRY_GW*0.5f + EXCLUDE_PAD)
                    continue;
            }
            else
            {
                if(centerZ > EXIT_GZ - EXIT_GW*0.5f - EXCLUDE_PAD && centerZ < EXIT_GZ + EXIT_GW*0.5f + EXCLUDE_PAD)
                    continue;
            }
            glPushMatrix();
            glTranslatef(xSide + proj*2.0f, sy, centerZ);
            glutSolidSphere(0.055f, 10, 8);
            glPopMatrix();
        }
    }
    glEnable(GL_LIGHTING);
}

/* * =========================================================================
 * CINEMA THEATRE CEILING SURFACE
 * =========================================================================
 * Renders the main ceiling plate above the audience, integrating the
 * floating light sources into a solid structural roof.
 */
static void drawCeiling()
{
    glEnable(GL_LIGHTING);
    // Dark grey/black ceiling color to maintain the cinema hall vibe
    glColor3f(0.05f, 0.05f, 0.05f);

    // X axis: -11 to 11 (Width: 22), Y axis: 7.3f (Slightly above lights), Z axis: -13.5 to 20.5 (Depth: 34)
    boxAt(0.0f, 7.3f, 3.5f, 22.0f, 0.1f, 34.0f);
}

/**
 * @brief Renders overhead ceiling lamp fixtures with animated emissive glow.
 */
static void drawCeilingLights()
{
    glDisable(GL_LIGHTING);
    float glow = 0.85f + 0.15f * std::sin(ledPulse * 1.5f);
    glColor3f(1.0f*glow, 0.96f*glow, 0.80f*glow);

    const float px[4] = {-3.5f, 3.5f, -1.2f, 1.2f};
    const float pz[4] = { 1.2f, 1.2f, -5.8f, -5.8f};

    for(int i=0; i<4; ++i)
    {
        glPushMatrix();
        glTranslatef(px[i], 7.2f, pz[i]);
        glutSolidSphere(0.18f, 18, 14);
        glPopMatrix();
        boxAt(px[i], 7.2f, pz[i], 0.55f, 0.08f, 0.55f);
    }
    glEnable(GL_LIGHTING);
}

/* * =========================================================================
 * FINAL STABLE CURVED SCREEN RENDERER (OpenCV Texture + Lighting Support)
 * =========================================================================
 * Dynamically updates textures per frame from video file if playing.
 */
static void drawCurvedScreen() {
    const float SCR_W = 14.0f, SCR_H = 5.5f, SCR_CX = 0.0f, SCR_BASE_Y = -0.5f, SCR_Z = 17.5f, CURVE_R = 16.0f;
    const int SEGS = 60;
    float halfAng = std::asin((SCR_W * 0.5f) / CURVE_R);

    // 1. Video frame processing
    bool textureLoaded = false;
//    if(videoPlaying && cap.isOpened()) {
//        cv::Mat frame;
//        cap >> frame;
//        if(frame.empty()) { cap.set(cv::CAP_PROP_POS_FRAMES, 0); cap >> frame; }
//        if(!frame.empty()) {
//            cv::flip(frame, frame, 0);
//            cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);
//
//            if(videoTextureID == 0) glGenTextures(1, &videoTextureID);
//
//            glEnable(GL_TEXTURE_2D);
//            glBindTexture(GL_TEXTURE_2D, videoTextureID);
//            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frame.cols, frame.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, frame.data);
//            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//            textureLoaded = true;
//        }
//    }

    // 2. Rendering loop
    glDisable(GL_CULL_FACE);
    for(int i = 0; i < SEGS; ++i) {
        float t0 = (float)i / SEGS, t1 = (float)(i + 1) / SEGS;
        float a0 = -halfAng + t0 * 2.0f * halfAng, a1 = -halfAng + t1 * 2.0f * halfAng;
        float x0 = SCR_CX + CURVE_R * std::sin(a0), z0 = SCR_Z + CURVE_R * (std::cos(a0) - std::cos(halfAng));
        float x1 = SCR_CX + CURVE_R * std::sin(a1), z1 = SCR_Z + CURVE_R * (std::cos(a1) - std::cos(halfAng));

        if(textureLoaded) {
            glDisable(GL_LIGHTING); // Disable lighting
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, videoTextureID);
            // 🟢 MAGIC LINE: Displays original video color without lighting interference
            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        } else {
            glEnable(GL_LIGHTING); // Enable lighting when screen is off
            glDisable(GL_TEXTURE_2D);
            glColor3f(0.9f, 0.9f, 0.9f); // Off-white
        }

        glBegin(GL_QUADS);
            glNormal3f(0.0f, 0.0f, 1.0f);
            if(textureLoaded) glTexCoord2f(t0, 0.0f); glVertex3f(x0, SCR_BASE_Y, z0);
            if(textureLoaded) glTexCoord2f(t1, 0.0f); glVertex3f(x1, SCR_BASE_Y, z1);
            if(textureLoaded) glTexCoord2f(t1, 1.0f); glVertex3f(x1, SCR_BASE_Y + SCR_H, z1);
            if(textureLoaded) glTexCoord2f(t0, 1.0f); glVertex3f(x0, SCR_BASE_Y + SCR_H, z0);
        glEnd();

        if(textureLoaded) glDisable(GL_TEXTURE_2D);
    }

    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING); // Restore lighting at the end
}

/**
 * @brief Renders the structured side walls containing acoustic paneling.
 */
static void drawSideWalls()
{
    const float WALL_X_R   =  11.00f;
    const float WALL_X_L   = -11.00f;
    const float WALL_H      =   9.00f;
    const float WALL_BASE_Y =  -1.36f;
    const float WALL_Z_FAR  =  20.50f;
    const float WALL_Z_NEA  = -13.50f;

    const int PANELS_Z = 8;
    const float GAP = 0.08f;
    float span = WALL_Z_FAR - WALL_Z_NEA;
    float panelDepth = (span - (PANELS_Z - 1) * GAP) / PANELS_Z;
    float panelH = WALL_H - 0.8f;
    float panelY = WALL_BASE_Y + 0.4f + panelH * 0.5f;

    auto drawSidePanels = [&](float xFace)
    {
        for(int i=0;i<PANELS_Z;++i)
        {
            float cz = WALL_Z_NEA + panelDepth*0.5f + i * (panelDepth + GAP);
            float px = xFace + (xFace > 0 ? -0.03f : 0.03f);
            glColor3f(0.20f, 0.18f, 0.16f);
            boxAt(px, panelY, cz, 0.06f, panelH, panelDepth);
            glColor3f(0.12f, 0.10f, 0.10f);
            boxAt(px + (xFace > 0 ? 0.03f : -0.03f), panelY, cz, 0.04f, panelH * 0.94f, panelDepth * 0.9f);
            glColor3f(0.24f, 0.20f, 0.16f);
            boxAt(px + (xFace > 0 ? 0.04f : -0.04f), panelY, cz, 0.01f, panelH * 0.96f, panelDepth*0.92f);
        }
        glColor3f(0.10f, 0.10f, 0.10f);
        float corniceY = WALL_BASE_Y + WALL_H - 0.12f;
        boxAt(xFace, corniceY, (WALL_Z_FAR + WALL_Z_NEA) * 0.5f, 0.14f, 0.24f, span + 0.2f);
        float skirtY = WALL_BASE_Y + 0.12f;
        boxAt(xFace, skirtY, (WALL_Z_FAR + WALL_Z_NEA) * 0.5f, 0.14f, 0.24f, span + 0.2f);
    };

    drawSidePanels(WALL_X_R);
    drawSidePanels(WALL_X_L);
}

/**
 * @brief Renders the theatre entrance gate with dynamic opening animation.
 * Includes a forced perspective corridor effect behind the door.
 */
static void drawEntryGate()
{
    const float WALL_X   =  9.00f;
    const float GW       =  2.50f;
    const float GH       =  3.20f;
    const float BASE_Y   = -1.36f;
    const float GZ_CTR   = -12.10f;

    const float GATE_CENTER_X = WALL_X - 1.25f; // 7.75f
    const float POST_SZ  = 0.28f;

    glEnable(GL_LIGHTING);

    // --- 1. Static Frame ---
    glColor3f(0.22f, 0.14f, 0.05f);
    boxAt(GATE_CENTER_X - (GW/2 + POST_SZ/2), BASE_Y + GH*0.5f, GZ_CTR, POST_SZ, GH + 0.28f, POST_SZ);
    boxAt(GATE_CENTER_X + (GW/2 + POST_SZ/2), BASE_Y + GH*0.5f, GZ_CTR, POST_SZ, GH + 0.28f, POST_SZ);
    boxAt(GATE_CENTER_X, BASE_Y + GH + 0.12f, GZ_CTR, GW + POST_SZ*2.0f, 0.25f, 0.28f);

    // --- 2. Fake Corridor (Illusion Box) ---
    // This sits in the empty space between the gate and the actual back wall, so no wall cutting is needed!

    // Carpet floor (Reddish-brown)
    glColor3f(0.4f, 0.15f, 0.1f);
    boxAt(GATE_CENTER_X, BASE_Y + 0.05f, GZ_CTR - 0.5f, GW, 0.1f, 1.0f);

    // Dark wall at the back of the corridor (hides the actual wall)
    glColor3f(0.02f, 0.01f, 0.01f);
    boxAt(GATE_CENTER_X, BASE_Y + GH*0.5f, GZ_CTR - 1.0f, GW, GH, 0.1f);

    // Masks to keep the corridor surroundings dark
    boxAt(GATE_CENTER_X - GW/2 + 0.05f, BASE_Y + GH*0.5f, GZ_CTR - 0.5f, 0.1f, GH, 1.0f); // Left
    boxAt(GATE_CENTER_X + GW/2 - 0.05f, BASE_Y + GH*0.5f, GZ_CTR - 0.5f, 0.1f, GH, 1.0f); // Right
    boxAt(GATE_CENTER_X, BASE_Y + GH - 0.05f, GZ_CTR - 0.5f, GW, 0.1f, 1.0f); // Ceiling

    // --- 3. Entry Label (Static) ---
    glDisable(GL_LIGHTING);
    glColor3f(1.00f, 0.88f, 0.18f);
    glRasterPos3f(GATE_CENTER_X - 0.4f, BASE_Y + GH + 0.35f, GZ_CTR + 0.1f);
    const char* t = "ENTRY";
    for(const char* c=t; *c; ++c) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
    glEnable(GL_LIGHTING);

    // --- 4. Animated Door (Opens from the right hinge) ---
    glPushMatrix();
        float pivotX = GATE_CENTER_X + GW/2; // Right hinge
        glTranslatef(pivotX, 0, GZ_CTR);
        glRotatef(-entryGateAngle, 0, 1, 0); // Swings inwards
        glTranslatef(-pivotX, 0, -GZ_CTR);

        glColor3f(0.26f, 0.17f, 0.07f);
        boxAt(GATE_CENTER_X, BASE_Y + GH*0.5f, GZ_CTR, GW, GH, 0.1f);
    glPopMatrix();
}

/**
 * @brief Renders the static exit gate located on the opposite side.
 */
static void drawExitGate()
{
    const float LW_X    = -11.00f;
    const float GW      =   2.25f;
    const float GH      =   3.20f;
    const float BASE_Y  =  -1.36f;
    const float GZ_CTR  =  13.10f;

    const float GZ0     = GZ_CTR - GW*0.5f;
    const float GZ1     = GZ_CTR + GW*0.5f;
    const float PSZ     = 0.25f;
    const float LT      = 0.22f;
    const float FT      = 0.08f;
    const float PROJ    = 0.26f;

    glEnable(GL_LIGHTING);
    glColor3f(0.12f, 0.12f, 0.14f);
    boxAt(LW_X + 0.04f, BASE_Y + GH * 0.5f, GZ_CTR, 0.05f, GH, GW);

    glColor3f(0.88f, 0.88f, 0.90f);
    boxAt(LW_X + 0.12f, BASE_Y + GH * 0.5f, GZ_CTR, 0.06f, GH - 0.02f, GW - 0.02f);

    glColor3f(0.68f, 0.68f, 0.70f);
    for(int d = 1; d <= 2; ++d)
    {
        float lineZ = GZ0 + d * (GW / 3.0f);
        boxAt(LW_X + 0.152f, BASE_Y + GH * 0.5f, lineZ, 0.004f, GH - 0.10f, 0.015f);
    }
    glEnable(GL_LIGHTING);

    glColor3f(0.26f, 0.17f, 0.07f);
    boxAt(LW_X + PROJ, BASE_Y + GH*0.5f, GZ0 - PSZ*0.5f, PROJ*2.0f, GH + PSZ, PSZ);
    boxAt(LW_X + PROJ, BASE_Y + GH*0.5f, GZ1 + PSZ*0.5f, PROJ*2.0f, GH + PSZ, PSZ);
    boxAt(LW_X + PROJ, BASE_Y + GH + LT*0.5f, GZ_CTR, PROJ*2.0f, LT, GW + PSZ*2.0f);

    float FX = LW_X + PROJ*2.0f;
    boxAt(FX, BASE_Y + GH*0.5f, GZ0 - PSZ*0.35f, 0.07f, GH + 0.22f, PSZ*0.70f + 0.04f);
    boxAt(FX, BASE_Y + GH*0.5f, GZ1 + PSZ*0.35f, 0.07f, GH + 0.22f, PSZ*0.70f + 0.04f);
    boxAt(FX, BASE_Y + GH + LT*0.30f, GZ_CTR, 0.07f, LT*0.65f + 0.10f, GW + PSZ*2.0f + 0.12f);

    cMetal();
    boxAt(LW_X + PROJ*0.6f, BASE_Y + GH*0.5f, GZ0, FT, GH, FT);
    boxAt(LW_X + PROJ*0.6f, BASE_Y + GH*0.5f, GZ1, FT, GH, FT);
    boxAt(LW_X + PROJ*0.6f, BASE_Y + GH,      GZ_CTR, FT, FT, GW);

    glDisable(GL_LIGHTING);
    float exit_dx = 0.18f;
    float exit_dy = 0.40f;
    const char* et = "EXIT";
    const float exitTextX = FX + exit_dx * 2.5f;
    glColor3f(1.00f, 0.88f, 0.18f);
    glRasterPos3f(exitTextX, BASE_Y + GH + exit_dy, GZ_CTR - 0.18f);
    for(const char* c=et;*c;++c) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
    glColor3f(1.00f, 0.92f, 0.40f);
    glRasterPos3f(exitTextX + 0.010f, BASE_Y + GH + exit_dy, GZ_CTR - 0.18f);
    for(const char* c=et;*c;++c) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
    glEnable(GL_LIGHTING);
}

/* * =========================================================================
 * ROTATING GLOWING OBJECTS (DUAL-POSITION)
 * =========================================================================
 * Renders two spinning diamonds symmetrically on either side of the exit gate.
 */
static void drawRotatingObject(float offsetZ)
{
    glDisable(GL_LIGHTING);

    // Z position and X fixed according to the gate's position
    const float OBJ_X = -10.20f;
    const float OBJ_Y =   1.20f;
    const float OBJ_Z =  13.10f + offsetZ; // Offset added to the gate's center (13.10)

    glPushMatrix();
        glTranslatef(OBJ_X, OBJ_Y, OBJ_Z);
        glRotatef(ledPulse * 1.5f * 180.0f / M_PI, 0.0f, 1.0f, 0.0f);
        glScalef(0.40f, 0.65f, 0.40f);

        float pulse = 0.80f + 0.20f * std::sin(ledPulse * 2.0f);

        glColor3f(1.00f * pulse, 0.82f * pulse, 0.12f * pulse);
        glutSolidOctahedron();

        glColor3f(1.0f, 1.0f, 0.90f);
        glPushMatrix();
            glScalef(0.4f, 0.4f, 0.4f);
            glutSolidOctahedron();
        glPopMatrix();
    glPopMatrix();

    glEnable(GL_LIGHTING);
}

/**
 * @brief Renders the solid rear wall behind the seating area with decorative panels.
 */
static void drawBackWall()
{
    const float WALL_Z    = -13.50f;
    const float WALL_H    =   9.00f;
    const float WALL_BY   =  -1.36f;
    const float WALL_XL   = -11.00f;
    const float WALL_XR   =  11.00f;
    const float WW        = WALL_XR - WALL_XL;
    const float THICK     =   0.40f;

    glEnable(GL_LIGHTING);
    glColor3f(0.16f, 0.16f, 0.18f);
    boxAt(0.0f, WALL_BY + WALL_H*0.5f, WALL_Z - THICK*0.5f, WW + 0.4f, WALL_H + 0.2f, THICK);

    const int PANELS = 6;
    const float GAP = 0.08f;
    const float panelW = (WW - (PANELS - 1) * GAP) / PANELS;
    const float panelH = WALL_H - 0.8f;
    const float panelY = WALL_BY + 0.4f + panelH * 0.5f;

    for(int i=0;i<PANELS;++i)
    {
        float cx = WALL_XL + panelW*0.5f + i * (panelW + GAP);
        glColor3f(0.20f, 0.18f, 0.16f);
        boxAt(cx, panelY, WALL_Z, panelW, panelH, 0.06f);
        glColor3f(0.12f, 0.10f, 0.10f);
        boxAt(cx, panelY, WALL_Z + 0.031f, panelW * 0.88f, panelH * 0.94f, 0.02f);
        glColor3f(0.24f, 0.20f, 0.16f);
        boxAt(cx, panelY, WALL_Z + 0.05f, panelW * 0.12f, panelH * 0.95f, 0.01f);
    }

    glColor3f(0.10f, 0.10f, 0.10f);
    boxAt(0.0f, WALL_BY + WALL_H - 0.12f, WALL_Z + 0.02f, WW + 0.2f, 0.24f, 0.08f);
    boxAt(0.0f, WALL_BY + 0.12f, WALL_Z + 0.02f, WW + 0.2f, 0.24f, 0.08f);
}

/**
 * @brief Complex geometrical rendering of a single cinema chair.
 * Includes seat folding mechanisms, cushioning, armrests, and cupholders.
 */
static void drawChair()
{
    glPushMatrix();
    {
        const float p0z=0.00f, p0y=0.00f;
        const float p1z=-0.14f, p1y=0.55f;
        const float p2z=0.30f,  p2y=1.20f;
        const int N=30;

        glPushMatrix();
        glTranslatef(0,-0.65f,-0.28f);
        glRotatef(-8.0f,1,0,0);

        for(int i=0; i<N; ++i)
        {
            float tm=(float)i/N+0.5f/N;
            float bz=(1-tm)*(1-tm)*p0z+2*(1-tm)*tm*p1z+tm*tm*p2z;
            float by=(1-tm)*(1-tm)*p0y+2*(1-tm)*tm*p1y+tm*tm*p2y;
            float dz=2*(1-tm)*(p1z-p0z)+2*tm*(p2z-p1z);
            float dy=2*(1-tm)*(p1y-p0y)+2*tm*(p2y-p1y);
            float ang=-(float)(atan2(dz,dy)*180.0/M_PI);

            float w=0.95f-0.30f*tm*tm;
            float d=0.18f-0.06f*tm;
            float slH=1.20f/N*1.06f;

            glColor3f(0.60f+0.28f*tm, 0.22f+0.26f*tm, 0.02f+0.08f*tm);
            glPushMatrix();
            glTranslatef(0,by,bz);
            glRotatef(ang,1,0,0);
            box(w,slH,d);
            glPopMatrix();
        }

        {
            float tm=1.0f;
            float bz=(1-tm)*(1-tm)*p0z+2*(1-tm)*tm*p1z+tm*tm*p2z;
            float by=(1-tm)*(1-tm)*p0y+2*(1-tm)*tm*p1y+tm*tm*p2y;
            cOrangeHi();
            softAt(0,by,bz, 0.70f,0.12f,0.16f);
        }

        for(int i=0; i<N; ++i)
        {
            float tm=(float)i/N+0.5f/N;
            float bz=(1-tm)*(1-tm)*p0z+2*(1-tm)*tm*p1z+tm*tm*p2z;
            float by=(1-tm)*(1-tm)*p0y+2*(1-tm)*tm*p1y+tm*tm*p2y;
            float dz=2*(1-tm)*(p1z-p0z)+2*tm*(p2z-p1z);
            float dy=2*(1-tm)*(p1y-p0y)+2*tm*(p2y-p1y);
            float ang=-(float)(atan2(dz,dy)*180.0/M_PI);
            float w=0.93f-0.28f*tm*tm;
            float slH=1.20f/N*1.03f;
            cShell();
            glPushMatrix();
            glTranslatef(0,by,bz-0.10f);
            glRotatef(ang,1,0,0);
            box(w,slH,0.04f);
            glPopMatrix();
        }

        float stitchT[]={0.22f,0.48f,0.72f};
        for(int s=0;s<3;s++)
        {
            float st=stitchT[s];
            float bz=(1-st)*(1-st)*p0z+2*(1-st)*st*p1z+st*st*p2z;
            float by=(1-st)*(1-st)*p0y+2*(1-st)*st*p1y+st*st*p2y;
            float dz=2*(1-st)*(p1z-p0z)+2*st*(p2z-p1z);
            float dy=2*(1-st)*(p1y-p0y)+2*st*(p2y-p1y);
            float ang=-(float)(atan2(dz,dy)*180.0/M_PI);
            float sw=0.88f-0.24f*st*st;
            cOrangeDk();
            glPushMatrix();
            glTranslatef(0,by,bz+0.10f);
            glRotatef(ang,1,0,0);
            box(sw,0.016f,0.012f);
            glPopMatrix();
        }

        for(int side=-1;side<=1;side+=2)
        {
            for(int i=0; i<N; ++i)
            {
                float tm=(float)i/N+0.5f/N;
                float bz=(1-tm)*(1-tm)*p0z+2*(1-tm)*tm*p1z+tm*tm*p2z;
                float by=(1-tm)*(1-tm)*p0y+2*(1-tm)*tm*p1y+tm*tm*p2y;
                float dz=2*(1-tm)*(p1z-p0z)+2*tm*(p2z-p1z);
                float dy=2*(1-tm)*(p1y-p0y)+2*tm*(p2y-p1y);
                float ang=-(float)(atan2(dz,dy)*180.0/M_PI);
                float w=0.95f-0.28f*tm*tm;
                float slH=1.20f/N*1.06f;
                cOrangeDk();
                glPushMatrix();
                glTranslatef(side*w*0.5f,by,bz);
                glRotatef(ang,1,0,0);
                box(0.04f,slH,0.20f);
                glPopMatrix();
            }
        }
        glPopMatrix();
    }

    {
        glPushMatrix();
        glTranslatef(0,-0.65f,-0.28f);
        glRotatef(-seatFoldAng,1,0,0);

        cOrange();
        boxAt(0,-0.07f,0.44f, 1.06f,0.15f,0.88f);
        cOrangeHi();
        softAt(0,0.01f,0.44f, 0.96f,0.09f,0.78f);

        cShell();
        boxAt(0,-0.15f,0.44f, 1.04f,0.05f,0.86f);
        glPopMatrix();
    }

    {
        for(int side=-1; side<=1; side+=2)
        {
            float sx = side * 0.68f;
            cShell();
            boxAt(sx,-0.44f,0.16f, 0.24f,0.56f,1.02f);

            if(side==1)
            {
                cOrange();
                softAt(sx,-0.17f,-0.10f, 0.20f,0.09f,0.55f);

                glPushMatrix();
                glTranslatef(sx,-0.155f,0.55f);
                glRotatef(-90, 1, 0, 0);

                cShell();
                glPushMatrix();
                glTranslatef(0, 0, -0.015f);
                cylZ(0.115f, 0.030f, 24);
                glPopMatrix();

                cCupRim();
                annularDisk(0.072f, 0.110f, 24);

                cShell();
                glPushMatrix();
                glTranslatef(0, 0, -0.110f);
                cylZOpen(0.070f, 0.110f, 20);
                glPopMatrix();

                glColor3f(0.05f, 0.05f, 0.06f);
                glPushMatrix();
                glTranslatef(0, 0, -0.112f);
                GLUquadric* q = gluNewQuadric();
                gluDisk(q, 0, 0.068f, 20, 1);
                gluDeleteQuadric(q);
                glPopMatrix();

                glPopMatrix();

                cMetalHi();
                glPushMatrix();
                glTranslatef(sx,-0.155f+0.018f,0.55f);
                glRotatef(-90, 1, 0, 0);
                annularDisk(0.108f, 0.118f, 24);
                glPopMatrix();
            }
            else
            {
                cOrange();
                softAt(sx,-0.17f,0.16f, 0.20f,0.09f,0.90f);
            }

            glPushMatrix();
            glTranslatef(sx,-0.38f,0.67f);
            cShell();
            glScalef(0.22f,0.54f,0.22f);
            glutSolidSphere(0.5f,14,10);
            glPopMatrix();

            glPushMatrix();
            glTranslatef(sx,-0.38f,-0.35f);
            cShell();
            glScalef(0.22f,0.54f,0.12f);
            glutSolidSphere(0.5f,14,10);
            glPopMatrix();
        }
    }
    glPopMatrix();
}

/**
 * @brief Renders the Heads-Up Display (HUD) overlay containing interactive instructions.
 */
static void drawHUD()
{
    int w=glutGet(GLUT_WINDOW_WIDTH);
    int h=glutGet(GLUT_WINDOW_HEIGHT);

    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix(); glLoadIdentity();
    gluOrtho2D(0,w,0,h);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix(); glLoadIdentity();

    glColor3f(0.75f,0.50f,0.25f);
    const char* lines[]={
        "a/A:rotX  b/B:rotY  c/C:rotZ",
        "i/j/k: translate   +/-: zoom",
        "x/y/z: camera      P: fold seat   V: play video   ESC: quit"
    };
    for(int i=0; i<3; ++i){
        glRasterPos2i(10,10+i*16);
        for(const char* c=lines[i]; *c; ++c)
            glutBitmapCharacter(GLUT_BITMAP_8_BY_13,*c);
    }

    glColor3f(seatFolded?0.9f:0.5f, seatFolded?0.5f:0.9f, 0.3f);
    const char* st=seatFolded?"Seat: FOLDED UP":"Seat: DOWN (open)";
    glRasterPos2i(10,h-20);
    for(const char* c=st; *c; ++c)
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13,*c);

    glColor3f(videoPlaying?0.3f:0.6f, videoPlaying?0.9f:0.6f, videoPlaying?0.3f:0.6f);
    const char* vs = videoPlaying ? "Video: PLAYING (OpenCV Engine Running)" : "Video: STOPPED  [V to play]";
    glRasterPos2i(10, h-38);
    for(const char* c=vs; *c; ++c)
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13,*c);

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
}

/* * =========================================================================
 * LIGHTING ENGINE CONFIGURATION
 * =========================================================================
 */

/**
 * @brief Initializes base lighting and material reflection parameters.
 */
static void setupLighting()
{
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);
    glEnable(GL_LIGHT3);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    GLfloat amb[]={0.05f,0.04f,0.03f,1.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb);

    GLfloat spec[]={0.35f,0.20f,0.08f,1.0f};
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec);
    glMaterialf (GL_FRONT_AND_BACK, GL_SHININESS, 26.0f);
}

/**
 * @brief Updates dynamic lighting properties, including the screen glow when playing.
 */
static void updateLighting()
{
    GLfloat p0[]={-2.2f, 7.3f, 1.0f, 1.0f};
    GLfloat d0[]={1.00f, 0.98f, 0.90f, 1.0f};
    GLfloat s0[]={1.00f, 0.98f, 0.90f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, p0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  d0);
    glLightfv(GL_LIGHT0, GL_SPECULAR, s0);

    GLfloat p1[]={2.2f, 7.3f, -5.5f, 1.0f};
    GLfloat d1[]={0.92f, 0.92f, 0.84f, 1.0f};
    GLfloat s1[]={0.95f, 0.95f, 0.90f, 1.0f};
    glLightfv(GL_LIGHT1, GL_POSITION, p1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE,  d1);
    glLightfv(GL_LIGHT1, GL_SPECULAR, s1);

    GLfloat p2[]={-8.0f, 2.0f, -2.0f, 1.0f};
    GLfloat d2[]={0.26f, 0.20f, 0.05f, 1.0f};
    glLightfv(GL_LIGHT2, GL_POSITION, p2);
    glLightfv(GL_LIGHT2, GL_DIFFUSE,  d2);

    GLfloat p3[]={8.0f, 2.0f, -2.0f, 1.0f};
    GLfloat d3[]={0.26f, 0.20f, 0.05f, 1.0f};
    glLightfv(GL_LIGHT3, GL_POSITION, p3);
    glLightfv(GL_LIGHT3, GL_DIFFUSE,  d3);

    if(videoPlaying && screenGlow > 0.1f)
    {
        glEnable(GL_LIGHT4);
        GLfloat p4[]={ 0.0f, 2.0f, -5.0f, 1.0f };
        GLfloat d4[]={ screenGlow * 0.4f, screenGlow * 0.38f, screenGlow * 0.28f, 1.0f };
        glLightfv(GL_LIGHT4, GL_POSITION, p4);
        glLightfv(GL_LIGHT4, GL_DIFFUSE,  d4);
    }
    else
    {
        glDisable(GL_LIGHT4);
    }
}

/* * =========================================================================
 * MASTER DISPLAY LOOP
 * =========================================================================
 */

/**
 * @brief The main display callback. Orchestrates the rendering of all scene elements.
 */
static void display()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(ex,ey,ez, 0,0,0, 0,1,0);
    updateLighting();

    glPushMatrix();
        glTranslatef(tx,ty,tz);
        glRotatef(degreeX,  0.5f,0,0);
        glRotatef(degreeY,  0,5.0f,0);
        glRotatef(degreeZ,  0,0,0.5f);
        glScalef(zoom,zoom,zoom);

        drawFloor();
        drawSideWalls();
        drawBackWall();
        drawFrontWall();

        drawSideLEDWall(-10.3f);
        drawSideLEDWall( 10.3f);

        drawCeiling();
        drawCeilingLights();
        drawCurvedScreen();

        drawRotatingObject(-1.8f);
        drawRotatingObject(1.8f);

        drawEntryGate();
        drawExitGate();

        const int cols = 8;
        const int rows = 10;
        const float spacingX = 1.18f;
        const float spacingZ = 1.22f;
        const float rowRise  = 0.38f;
        const float frontZ   = 1.20f;

        float startX = -((cols-1)*spacingX)*0.5f;

        for(int r=0; r<rows; ++r)
        {
            float topY = -1.36f + (r + 1) * rowRise;
            float y = topY + 0.80f;
            float z = frontZ - r * spacingZ;

            for(int c=0; c<cols; ++c)
            {
                glPushMatrix();
                float x = startX + c*spacingX;
                float stagger = (r%2==0) ? -0.02f : 0.02f;
                glTranslatef(x + stagger, y, z);
                drawChair();
                glPopMatrix();
            }
        }
    glPopMatrix();

    drawHUD();
    glutSwapBuffers();
}

/* * =========================================================================
 * USER INTERACTION (KEYBOARD & TIMER LOGIC)
 * =========================================================================
 */

/**
 * @brief Handles primary keyboard inputs (movement, media control, lighting).
 */
// 🟢 OpenCV Video Streaming Keyboard Interaction Code
static void key(unsigned char k, int x, int y)
{
    switch(k)
    {
        case 27: case 'q':
          //  if(cap.isOpened()) cap.release();
            exit(0);
            break;

        case 'a': degreeX+=2.5f; break;  case 'A': degreeX-=2.5f; break;
        case 'b': degreeY+=2.5f; break;  case 'B': degreeY-=2.5f; break;
        case 'c': degreeZ+=2.5f; break;  case 'C': degreeZ-=2.5f; break;

        case 'i': tx+=0.2f; break;  case 'I': tx-=0.2f; break;
        case 'j': ty+=0.2f; break;  case 'J': ty-=0.2f; break;
        case 'k': tz+=0.2f; break;  case 'K': tz-=0.2f; break;

        case '+': zoom+=0.05f; break;
        case '_': zoom-=0.05f; break;

        case 'x': ex++; break;  case 'X': ex--; break;
        case 'y': ey++; break;  case 'Y': ey--; break;
        case 'z': ez++; break;  case 'Z': ez--; break;

        case 'p': case 'P':
            seatFolded=!seatFolded;
            break;

        case 'v': case 'V':
        {
            videoPlaying = !videoPlaying;
            if(videoPlaying)
            {
                cap.open(VIDEO_FILE);
                if(!cap.isOpened()) {
                    printf("\n❌ Error: Could not open video file at %s\n", VIDEO_FILE);
                    videoPlaying = false;
                } else {
                    if(videoTextureID == 0) {
                        glGenTextures(1, &videoTextureID);
                    }

                    // 🟢 MAGIC LINE: Silently starts playing the video's audio track in the background
                    char soundCmd[512];
                    snprintf(soundCmd, sizeof(soundCmd), "ffplay -nodisp -autoexit \"%s\" &", VIDEO_FILE);
                    system(soundCmd);
                }
                screenGlow = 0.0f;
            }
            else
            {
                cap.release();
                // 🔴 Stops background sound when video playback stops
                system("pkill ffplay");
                screenGlow = 0.0f;
            }
            break;
        }

        case '1': glEnable(GL_LIGHT0); break;
        case '2': glEnable(GL_LIGHT1); break;
        case '3': glEnable(GL_LIGHT2); break;
        case '4': glEnable(GL_LIGHT3); break;

        case '0': glDisable(GL_LIGHT0); break;
        case '9': glDisable(GL_LIGHT1); break;
        case '8': glDisable(GL_LIGHT2); break;
        case '7': glDisable(GL_LIGHT3); break;

        case 'e': case 'E':
            entryGateOpen = !entryGateOpen;
            break;
    }
    glutPostRedisplay();
}

/**
 * @brief Handles special keyboard inputs like directional arrow keys for rotation.
 */
static void specialKeys(int k, int x, int y)
{
    switch(k){
        case GLUT_KEY_UP:    degreeX-=3.0f; break;
        case GLUT_KEY_DOWN:  degreeX+=3.0f; break;
        case GLUT_KEY_LEFT:  degreeY-=3.0f; break;
        case GLUT_KEY_RIGHT: degreeY+=3.0f; break;
    }
    glutPostRedisplay();
}

/**
 * @brief The universal timer callback that drives animations (led pulse, chair folds, gate swing).
 */
void timerFunc(int)
{
    ledPulse += 0.05f;
    if(ledPulse > 6.28318f) ledPulse -= 6.28318f;

    float target = seatFolded ? 85.0f : 0.0f;
    seatFolded ? 85.0f : 0.0f;
    seatFoldAng += (target - seatFoldAng) * 0.12f;

    if(videoPlaying)
    {
        if(screenGlow < 1.0f) screenGlow += 0.025f;
        screenScanLine += 0.04f;
        if(screenScanLine > 6.28318f) screenScanLine -= 6.28318f;
    }
    else
    {
        if(screenGlow > 0.0f) screenGlow -= 0.04f;
        if(screenGlow < 0.0f) screenGlow = 0.0f;
    }

    glutPostRedisplay();
    glutTimerFunc(16, timerFunc, 0);

    // --- ENTRY GATE ANIMATION ---
    float targetEntry = entryGateOpen ? 90.0f : 0.0f;
    entryGateAngle += (targetEntry - entryGateAngle) * 0.1f;
}

/* * =========================================================================
 * APPLICATION ENTRY POINT
 * =========================================================================
 */
int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(960,720);
    glutInitWindowPosition(80,60);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Star Cineplex");

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutSpecialFunc(specialKeys);
    glutTimerFunc(16, timerFunc, 0);

    glClearColor(0.03f, 0.02f, 0.01f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);

    glEnable(GL_CULL_FACE);
    glCustomize: glCullFace(GL_BACK);

    setupLighting();

    glutMainLoop();
    return EXIT_SUCCESS;
}
