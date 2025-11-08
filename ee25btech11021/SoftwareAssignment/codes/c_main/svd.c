#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void find_svd(double *a,double *b,int m,int n,int iter,int k){
  double *v = malloc(n*sizeof(double));
  double *u = malloc(m*sizeof(double));
  double *av = malloc(m*sizeof(double));
/*allocating memory to the pointers */
  for(int comp = 0;comp <k;comp++){//loop for forming vectors in the matrix v and u .

    for(int i=0;i<n;i++){//loop for taking some random vector for v
        v[i] = (double)rand()/RAND_MAX;
    }
    for(int z = 0;z<iter;z++){//this loop is used for iteration to find eigen vector

    for(int i=0;i<m;i++){
    av[i] = 0;
    for(int j=0;j<n;j++){
        av[i] += a[i*n+j]* v[j];//multipling matrix A with v
}
    }
    double *new_v = malloc(n*sizeof(double));
    for(int j=0;j<n;j++){
    new_v[j] = 0;
     for(int i=0;i<m;i++){
    new_v[j] += a[i*n +j]*av[i];//multipling matrix A^T with above formed matrix
     }

     }
      double norm = 0;
        for(int i=0;i<n;i++){//making the vector v unit.
            norm += new_v[i]*new_v[i];
        }
        norm = sqrt(norm);
        for(int i=0;i<n;i++){
            v[i] = new_v[i]/norm;
        }
    }
     for(int j=0;j<m;j++){//now we get eigen vector v which is multiplied with A to get vector u
     u[j]= 0;
         for(int i=0;i<n;i++){
        u[j] += a[j *n +i]*v[i];
}
     }
     double sigma = 0;//finding sigma value
for(int i=0;i<m;i++){
       sigma += av[i]*av[i];
}
sigma = sqrt(sigma);

for(int i=0;i<m;i++)
    u[i] = av[i]/(sigma);
        for(int i=0;i<m;i++){
         for(int j=0;j<n;j++){
             double component = u[i]*sigma*v[j];
        b[i*n+j] += component;//finding new matrix
        a[i*n+j] -= component;//Substracting from original matrix to continue aliteration
            }}
  }
     }
    double f_error(double *a,double *b,int n,int m){//for calculating error
    double k = 0;
    double sum = 0;
        for(int i = 0;i<m*n;i++){
        k = a[i] - b[i];
        k = k*k;
        sum += k;
            }
            return sqrt(sum);
    }

void imagep(const char *B, int *k, int b) {
    int n, m, c;
    unsigned char *input = stbi_load(B, &n, &m, &c, 1);//loading the image
    if (!input) {
        printf("Error: could not load %s\n", B);//if there was no image in the same file this runs
        return;
    }


    double *A = malloc(n * m * sizeof(double));
    for (int i = 0; i < n * m; i++) {
        A[i] = (double)input[i];//makinig the array from char datatype to double datatype
    }
stbi_image_free(input);
    double *A_new = malloc(n * m * sizeof(double));
    double *A_out = malloc(n * m * sizeof(double));

    for (int x = 0; x < b; x++) {
        int p = k[x];


        for (int i = 0; i < n * m; i++) {
        A_out[i] = 0;//intiate with all 0
            A_new[i] = A[i];//making a copy of matrix A so that original can be used later
        }

       printf("RANK = %d for %s\n",p,B);
        find_svd(A_new, A_out, m, n, 50, p);//implementing the fuction mentioned above
        double err= f_error(A,A_out,n,m );
printf(" frobenious error = %f\n",err);

        unsigned char *output = malloc(m * n * sizeof(unsigned char));
       double min = 1e9, max = -1e9;
for (int i = 0; i < m*n; i++) {
    if (A_out[i] < min) min = A_out[i];
    if (A_out[i] > max) max = A_out[i];
}
for (int i = 0; i < m*n; i++) {
    output[i] = (unsigned char)(255 * (A_out[i] - min) / (max - min + 1e-9));
}
char result[300];
        sprintf(result, "%s_out_%d.jpg", B, p);
        stbi_write_jpg(result, n, m, 1, output, 100);//write the array into image file
printf("Image saved_%s_%d\n",B,p);
        free(output);
        printf("------end------\n");
    }
}
int main() {
    const char *imgs[] = {"image1.jpg", "image2.jpg", "image3.jpg"};
    int K[] = {5, 20, 50, 100};
    for (int i = 0; i < 3; i++)
        imagep(imgs[i],K,4);
    return 0;
}
