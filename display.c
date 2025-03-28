#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "raylib.h"

// Particle structure with position and velocity
typedef struct {
    Vector2 position;
    Vector2 velocity;
} Particle;

// Particle mesh structure
typedef struct {
    int rows;
    int cols;
    float radius;
    Particle **particles;
} ParticleMesh;

// Function to create a particle mesh with random directions but constant velocity magnitude
ParticleMesh createParticleMesh(int rows, int cols, float radius, float spacingX, float spacingY, float offsetX, float offsetY, float v_0) {
    ParticleMesh mesh;
    mesh.rows = rows;
    mesh.cols = cols;
    mesh.radius = radius;

    mesh.particles = (Particle **)malloc(rows * sizeof(Particle *));
    if (mesh.particles == NULL) {
        perror("Failed to allocate memory for particle rows");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < rows; i++) {
        mesh.particles[i] = (Particle *)malloc(cols * sizeof(Particle));
        if (mesh.particles[i] == NULL) {
            perror("Failed to allocate memory for particle columns");
            exit(EXIT_FAILURE);
        }

        for (int j = 0; j < cols; j++) {
            float x = j * spacingX + offsetX;
            float y = i * spacingY + offsetY;
            mesh.particles[i][j].position = (Vector2){x, y};

            // Generate a random direction (angle between 0 and 2 * PI)
            float angle = (float)(rand() % 360) * (PI / 180.0f);  // Random angle in radians

            // Calculate the velocity components based on the random angle and constant speed
            mesh.particles[i][j].velocity.x = v_0 * cos(angle);
            mesh.particles[i][j].velocity.y = v_0 * sin(angle);
        }
    }

    return mesh;
}

// Function to free allocated memory
void freeParticleMesh(ParticleMesh mesh) {
    for (int i = 0; i < mesh.rows; i++) {
        free(mesh.particles[i]);
    }
    free(mesh.particles);
}

// Function to update the particle mesh, apply velocity, and handle collisions.
// The parameter 'totalParticles' indicates the actual number of particles (from the drawn subset).
void updateParticleMesh(ParticleMesh *mesh, float screenWidth, float screenHeight, int totalParticles) {
    int cols = mesh->cols;
    // First, update positions and handle boundary collisions for each particle
    for (int a = 0; a < totalParticles; a++) {
        int i = a / cols;
        int j = a % cols;
        Particle *p = &mesh->particles[i][j];

        // Update position based on velocity
        p->position.x += p->velocity.x;
        p->position.y += p->velocity.y;

        // Handle boundary collisions (screen edges)
        if (p->position.x - mesh->radius < 0 || p->position.x + mesh->radius > screenWidth) {
            p->velocity.x = -p->velocity.x;
        }
        if (p->position.y - mesh->radius < 0 || p->position.y + mesh->radius > screenHeight) {
            p->velocity.y = -p->velocity.y;
        }
    }

    // Now, process collisions between each unique pair of particles
    for (int a = 0; a < totalParticles; a++) {
        int i = a / cols;
        int j = a % cols;
        Particle *p = &mesh->particles[i][j];

        for (int b = a + 1; b < totalParticles; b++) {
            int k = b / cols;
            int l = b % cols;
            Particle *p2 = &mesh->particles[k][l];

            // Calculate the vector between particles
            float dx = p2->position.x - p->position.x;
            float dy = p2->position.y - p->position.y;
            float distance = sqrt(dx * dx + dy * dy);

            // Check if the particles are colliding (distance < 2 * radius)
            if (distance < 2 * mesh->radius && distance > 0.0f) {
                // Calculate the normal vector (unit vector)
                Vector2 normal = { dx / distance, dy / distance };

                // Relative velocity along the normal direction
                Vector2 relativeVelocity = { p->velocity.x - p2->velocity.x, p->velocity.y - p2->velocity.y };
                float velocityAlongNormal = relativeVelocity.x * normal.x + relativeVelocity.y * normal.y;

                // Only resolve if particles are moving towards each other
                if (velocityAlongNormal < 0) {
                    float restitution = 1.0f;  // Elastic collision

                    // Impulse scalar (for equal masses)
                    float impulse = -(1 + restitution) * velocityAlongNormal / 2.0f;

                    // Apply impulse to both particles
                    p->velocity.x += impulse * normal.x;
                    p->velocity.y += impulse * normal.y;

                    p2->velocity.x -= impulse * normal.x;
                    p2->velocity.y -= impulse * normal.y;
                }

                // Positional correction to prevent sinking (overlap)
                float penetration = 2 * mesh->radius - distance;
                float percent = 0.5f;  // Correction factor: each particle moves half the penetration depth
                float correction = percent * penetration;
                p->position.x -= correction * normal.x;
                p->position.y -= correction * normal.y;
                p2->position.x += correction * normal.x;
                p2->position.y += correction * normal.y;
            }
        }
    }
}

int main() {
    int N = 350; // Total number of particles
    float screenWidth = 800.0;
    float screenHeight = 600.0;
    float radius = 5; // Particle radius
    float v_0 = 3.0f; // Constant initial velocity magnitude

    // Calculate the number of rows and columns required to fit exactly N particles
    int cols = (int)ceil(sqrt(N)); // Initial guess for columns
    int rows = (N + cols - 1) / cols; // Corresponding rows to fit exactly N particles

    // Calculate proper spacing to ensure at least a 2-radius margin on all sides
    float margin = 2 * radius;
    float spacingX = (screenWidth - margin * 2) / (cols - 1);
    float spacingY = (screenHeight - margin * 2) / (rows - 1);

    // Offsets to center the particles in the window
    float offsetX = margin;
    float offsetY = margin;

    InitWindow(screenWidth, screenHeight, "Hard Sphere Gas Model");
    SetTargetFPS(60);

    ParticleMesh Mesh = createParticleMesh(rows, cols, radius, spacingX, spacingY, offsetX, offsetY, v_0);

    while (!WindowShouldClose()) {
        // Update particle positions, boundary collisions, and particle-particle collisions.
        updateParticleMesh(&Mesh, screenWidth, screenHeight, N);

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText("Hard Sphere Gas Model", 280, 50, 20, DARKGRAY);

        // Draw the particles (only draw the first N particles)
        for (int a = 0; a < N; a++) {
            int i = a / cols;
            int j = a % cols;
            DrawCircleV(Mesh.particles[i][j].position, radius, RED);
        }

        EndDrawing();
    }

    CloseWindow();
    freeParticleMesh(Mesh);
    return 0;
}
