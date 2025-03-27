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

//DEFINE COLLISION LOGIC

Float2DArray collisionVelocities(Float2DArray arrVelocities){
    return arrVelocities;
}

int main(){

    const double PI = 3.141592653589793;

    int i;
    int j;
    int k;

    //PARAMETERS

    int N = 100;
    float L = 10.0;
    float radius = 0.2;
    //float v_0 = 2.0;
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

    float mesh[points_number][points_number]; //ROWS & COLUMNS

    for (j=0;j<points_number;j++){
        for (k=0;k<points_number;k++){
            mesh[j][k]=points[k];
        }
    }

    Float2DArray arrPostion;
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
    printf("\n");

    printf("%ld\n\n", sizeof(mesh)/sizeof(mesh[0][0]));

    //INITIALIZE PARTICLE VELOCITIES

    int particle_number = mesh_points*mesh_points;

    float theta[particle_number];

    srand(time(NULL));

    for (i=0;i<particle_number;i++){
        float random_value = ((float)rand() / RAND_MAX) * (2 * PI);
        theta[i]=random_value;
    }

    
    printf("%ld\n\n", sizeof(theta)/sizeof(theta[0]));

    float velocities[particle_number][2]; //ROWS & COLUMNS



    printf("%ld\n\n", sizeof(velocities)/sizeof(velocities[0]));

    Float2DArray arrVelocities;
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

    /*for (i = 0; i < N; i++) {
        for (j = 0; j < 2; j++) {
            printf("%f ", arrVelocities.data[i][j]);
        }
        printf("\n");
    }*/



    return 0;

}