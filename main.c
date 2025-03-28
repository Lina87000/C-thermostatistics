#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define M 100

//DEFINE DATA TYPES

//2D float array
typedef struct {
    float **data;
    int rows;
    int cols;
} Float2DArray;

//Particle coordinates array
typedef struct {
    float x;
    float y;
} Particle;

//Particle mesh array
typedef struct {
    Particle **particles; // 2D array of particles.
    int rows;
    int cols;
    float radius;
} ParticleMesh;

typedef struct {
    float **velocities; // 2D array [N x 2]
    int N;
} VelocityArray;

//CREATE PARTICLE MESH

ParticleMesh createParticleMesh(int rows, int cols, float radius, float spacing) {
    ParticleMesh mesh;
    mesh.rows = rows;
    mesh.cols = cols;
    mesh.radius = radius;

    mesh.particles = malloc(rows * sizeof(Particle *));
    if (mesh.particles == NULL) {
        perror("Failed to allocate memory for particle rows");
        exit(EXIT_FAILURE);
    }
    
    for (int i = 0; i < rows; i++) {
        mesh.particles[i] = malloc(cols * sizeof(Particle));
        if (mesh.particles[i] == NULL) {
            perror("Failed to allocate memory for particle columns");
            exit(EXIT_FAILURE);
        }

        for (int j = 0; j < cols; j++) {
            mesh.particles[i][j].x = j * (spacing) + radius;
            mesh.particles[i][j].y = i * (spacing) + radius;
        }
    }
    
    return mesh;
}

// Function to print the particle mesh coordinates.
void printParticleMesh(ParticleMesh mesh) {
    for (int i = 0; i < mesh.rows; i++) {
        for (int j = 0; j < mesh.cols; j++) {
            printf("(%.2f, %.2f) ", mesh.particles[i][j].x, mesh.particles[i][j].y);
        }
        printf("\n");
    }
}

void freeParticleArray(ParticleMesh mesh) {
    for (int i = 0; i < mesh.rows; i++) {
        free(mesh.particles[i]);
    }
    free(mesh.particles);
}

//INITIALIZE VELOCITY ARRAYS

float randomFloat(float min, float max) {
    return min + ((float)rand() / RAND_MAX) * (max - min);
}

VelocityArray createVelocityArray(int num_particles, float LIM, float v_0) {
    VelocityArray vel;
    vel.N = num_particles;

    // Allocate memory for velocity array (N x 2)
    vel.velocities = malloc(num_particles * sizeof(float *));
    if (vel.velocities == NULL) {
        perror("Failed to allocate memory for velocity array");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < num_particles; i++) {
        vel.velocities[i] = malloc(2 * sizeof(float)); // Each row has 2 elements (v_x, v_y)
        if (vel.velocities[i] == NULL) {
            perror("Failed to allocate memory for velocity components");
            exit(EXIT_FAILURE);
        }
        float theta = randomFloat(0, 2 * LIM);  // Generate a random angle from 0 to 2π
        vel.velocities[i][0] = v_0 * cos(theta); // v_x
        vel.velocities[i][1] = v_0 * sin(theta); // v_y
    }

    return vel;
}

// Function to print the particle mesh coordinates.
void printVelocities(VelocityArray vel) {
    for (int i = 0; i < vel.N; i++) {
        printf("Particle %d: v_x = %f, v_y = %f\n", i, vel.velocities[i][0], vel.velocities[i][1]);
    }
}


void freeVelocityArray(VelocityArray vel) {
    for (int i = 0; i < vel.N; i++) {
        free(vel.velocities[i]);
    }
    free(vel.velocities);
}

//DEFINE COLLISION LOGIC

/*Float2DArray collisionVelocities(Float2DArray arrVelocities){

    Float2DArray arrVelocities;
    arrVelocities.data = data;
    arrVelocities.cols = cols;

    return arrVelocities;
}*/

int main(){

    const double PI = 3.141592653589793;

    int i;
    //int j;
    //int k;

    //PARAMETERS

    int N = 100;
    float L = 10.0;
    float radius = 0.2;
    float v_0 = 2.0;
    /*float mass = 1.0;
    float duration = 10.0;
    float dt = 0.01;*/

    //INITIALIZE PARTICLE MESH

    float mesh_points = ceil(sqrt(N));
    float spacing = L/mesh_points;
   
    int points_number = trunc(mesh_points);
    float points[points_number];
    printf("%d\n\n",points_number);


    for (i=0; i<points_number; i++){
        points[i] = (radius+(spacing/2)*i);
    }

    printf("%ld\n\n", sizeof(points)/sizeof(points[0]));

    /*float mesh[points_number][points_number]; //ROWS & COLUMNS

    for (j=0;j<points_number;j++){
        for (k=0;k<points_number;k++){
            mesh[j][k]=points[k];
        }
    }*/

    ParticleMesh mesh = createParticleMesh(points_number, points_number, radius, spacing);
    printf("\n");
    printParticleMesh(mesh);
    freeParticleArray(mesh);

    srand(time(NULL));
    VelocityArray velocities = createVelocityArray(N, PI, v_0);
    printVelocities(velocities);
    freeVelocityArray(velocities);

    /*Float2DArray arrPostion;
    arrPostion.cols = points_number;
    arrPostion.rows = points_number;

    arrPostion.data = malloc(points_number * sizeof(float *));
    if (arrPostion.data == NULL) {
        perror("Failed to allocate memory for rows");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < points_number; i++) {
        arrPostion.data[i] = malloc(2 * sizeof(float));
        if (arrPostion.data[i] == NULL) {
            perror("Failed to allocate memory for columns");
            exit(EXIT_FAILURE);
        }
    }

    for (i=0;i<points_number;i++){
        arrPostion.data[i][0]=cos(points[i]);
    }

    for (i=0;i<N;i++){
        arrVelocities.data[i][1]=sin(theta[i]);
    }


    for (i = 0; i < points_number; i++) {
        for (j = 0; j < 2; j++) {
            printf("%f ", arrPostion.data[i][j]);
        }
        printf("\n");
    }
    printf("\n");*/

    //printf("%ld\n\n", sizeof(mesh)/sizeof(mesh[0][0]));

    //INITIALIZE PARTICLE VELOCITIES

    int particle_number = mesh_points*mesh_points;

    float theta[particle_number];

    //srand(time(NULL));

    for (i=0;i<particle_number;i++){
        float random_value = ((float)rand() / RAND_MAX) * (2 * PI);
        theta[i]=random_value;
    }

    
    printf("%ld\n\n", sizeof(theta)/sizeof(theta[0]));

    //float velocities[particle_number][2]; //ROWS & COLUMNS



    //printf("%ld\n\n", sizeof(velocities)/sizeof(velocities[0]));

    /*Float2DArray arrVelocities;
    arrVelocities.cols = 2;
    arrVelocities.rows = N;

    arrVelocities.data = malloc(N * sizeof(float *));
    if (arrVelocities.data == NULL) {
        perror("Failed to allocate memory for rows");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < N; i++) {
        arrVelocities.data[i] = malloc(2 * sizeof(float));
        if (arrVelocities.data[i] == NULL) {
            perror("Failed to allocate memory for columns");
            exit(EXIT_FAILURE);
        }
    }

    for (i=0;i<N;i++){
        arrVelocities.data[i][0]=cos(theta[i]);
    }

    for (i=0;i<N;i++){
        arrVelocities.data[i][1]=sin(theta[i]);
    }

    arrVelocities = collisionVelocities(arrVelocities);
    */
    /*for (i = 0; i < N; i++) {
        for (j = 0; j < 2; j++) {
            printf("%f ", arrVelocities.data[i][j]);
        }
        printf("\n");
    }*/

    return 0;

}