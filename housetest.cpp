#include<stdio.h>
#include<stdlib.h>

void multiply(double **a, double **b, double **result, int r1, int c1, int r2, int c2) {
    
    for (int i = 0; i < r1; ++i) {
        for (int j = 0; j < c2; ++j) {
            result[i][j] = 0;
        }
    }

    for (int i =0; i<r1; i++) {
        for (int j=0; j<c2; j++) {
            for (int k=0; k<c1; k++) {
                result[i][j] += a[i][k] * b[k][j];
            }
        }
    }

}

void inverse(double **m1, double **id, int dim) {
    int i, j, k, p;
    double f=0;
    for(p = 0; p<dim; p++) {
        f = m1[p][p];
        //printf("%lf ", f);
        for (j = 0; j<dim; j++) {
            m1[p][j] = (m1[p][j])/f;
            id[p][j] = (id[p][j])/f;
        }
        
        for (i=p+1; i<dim; i++) {
            f = m1[i][p];
            for(k = 0; k<dim; k++) {
                m1[i][k] = (m1[i][k])-(m1[p][k])*f;
                id[i][k] = (id[i][k])-(id[p][k])*f;
            }
        }
    }
    for(p=dim-1; p>=0; p--) {
        for(i=p-1; i>=0; i--) {
            f=m1[i][p];
            for(int l=0; l<dim; l++){
                m1[i][l] = (m1[i][l])-(m1[p][l])*f;
                id[i][l] = (id[i][l])-(id[p][l])*f; 
            }
        }
    }
    }

int main(int argc, char** argv) { 

    int erowdim = 0;
    int ecoldim = 0;
    int drowdim = 0;
    int dcoldim = 0;
    char string[100];

    int i;
    int j;



    //estimate
    FILE* est;
    est=fopen(argv[1],"r");
    if(est==NULL){
        printf("error\n");
    }
	if (!est) {
		printf("error\n");
		return 0;
	}

    fscanf(est, "%s", string);
    //printf("%s", string);
    fscanf(est, "%d", &ecoldim);
    //printf("%d", erowdim);
    fscanf(est, "%d", &erowdim);
    //printf("%d", ecoldim);

    ecoldim += 1;

    int erowdimt = ecoldim;
    int ecoldimt = erowdim;


    //og
    double ** og = (double ** )malloc(sizeof(double *) * erowdim);
        for (i=0; i<erowdim; i++) {
            og[i] = (double*)malloc(sizeof(double) * ecoldim);
        }
    //x
    double ** x = (double ** )malloc(sizeof(double *) * erowdim);
        for (i=0; i<erowdim; i++) {
            x[i] = (double*)malloc(sizeof(double) * ecoldim);
        }
    //xt
    double ** xt = (double ** )malloc(sizeof(double *) * erowdimt);
        for (i=0; i<erowdimt; i++) {
            xt[i] = (double*)malloc(sizeof(double) * ecoldimt);
        }
    //xtx
    double ** xtx = (double ** )malloc(sizeof(double *) * erowdimt);
        for (i=0; i<erowdimt; i++) {
            xtx[i] = (double*)malloc(sizeof(double) * ecoldim);
        }
    //id
    double ** id = (double ** )malloc(sizeof(double *) * erowdimt);
        for (i=0; i<erowdimt; i++) {
            id[i] = (double*)malloc(sizeof(double) * ecoldim);
        }
    //y
    double ** y = (double ** )malloc(sizeof(double *) * erowdim);
        for (i=0; i<erowdim; i++) {
            y[i] = (double*)malloc(sizeof(double) * 1);
        }
    //idxt
    double ** idxt = (double ** )malloc(sizeof(double *) * erowdimt);
        for (i=0; i<erowdimt; i++) {
            idxt[i] = (double*)malloc(sizeof(double) * ecoldimt);
        }

    //idxty
    double ** idxty = (double ** )malloc(sizeof(double *) * erowdimt);
        for (i=0; i<erowdimt; i++) {
            idxty[i] = (double*)malloc(sizeof(double) * 1);
        }

    //erowdim += 1;

    //og
    for (i=0; i<erowdim; i++){
		for (j=0; j<ecoldim; j++) {

			    fscanf(est, "%lf", &og[i][j]);
            
		}
	}
    
    //x
    for (i=0; i<erowdim; i++){
		for (j=0; j<ecoldim; j++) {
            if (j==0) {
                x[i][j] = 1;
            }
            
            else {
			    x[i][j]=og[i][j-1];
            }
		}
	}
    //y
    for (i=0; i<erowdim; i++){
		for (j=0; j<ecoldim; j++) {
            if (j==ecoldim-1) {
                //printf("%lf ", og[i][j]);
                y[i][0] = og[i][j];
            }
            
		}
	}     
    
    //xt
    for (i=0; i<erowdimt; i++){
		for (j=0; j<ecoldimt; j++) {
            xt[i][j] = x[j][i];
		}
	}

    //id
    for (i=0; i<erowdimt; i++){
		for (j=0; j<ecoldim; j++) {
            if (i==j) {
                id[i][j] = 1;
            }
            else {
                id[i][j] = 0;
            }
		}
	}


    multiply(xt, x, xtx, erowdimt, ecoldimt, erowdim, ecoldim);

    inverse(xtx, id, erowdimt);

    multiply(id, xt, idxt, erowdimt, ecoldim, erowdimt, ecoldimt);

    multiply(idxt, y, idxty, erowdimt, ecoldimt, erowdim, 1);

    //data
    FILE* data;
    data=fopen(argv[2],"r");
    if(data==NULL){
        printf("error\n");
    }
	if (!data) {
		printf("error\n");
		return 0;
	}

    fscanf(data, "%s", string);
    //printf("%s\n", string);
    fscanf(data, "%d", &dcoldim);
    //printf("%d\n", dcoldim);
    fscanf(data, "%d", &drowdim);
    //printf("%d\n", drowdim);

    int dcoldimx = dcoldim + 1;


    //ogd
    double ** ogd = (double ** )malloc(sizeof(double *) * drowdim);
        for (i=0; i<drowdim; i++) {
            ogd[i] = (double*)malloc(sizeof(double) * dcoldimx);
        }
    //xd
    double ** xd = (double ** )malloc(sizeof(double *) * drowdim);
    for (i=0; i<drowdim; i++) {
        xd[i] = (double*)malloc(sizeof(double) * dcoldimx);
    }

    //ogd
    for (i=0; i<drowdim; i++){
		for (j=0; j<dcoldimx; j++) {
                if (j==0) {
                    ogd[i][j] = 1;
                }
                else {
			        fscanf(data, "%lf", &ogd[i][j]);
                } 
		}
	}
    /*
    printf("ogd\n");
    for (i=0; i<drowdim; i++){
		for (j=0; j<dcoldimx; j++) {
            printf("%lf ", ogd[i][j]);
		}
        printf("\n");
	}
    */
    double ** final = (double ** )malloc(sizeof(double *) * drowdim);
    for (i=0; i<drowdim; i++) {
        final[i] = (double*)malloc(sizeof(double) * 1);
    }

    multiply(ogd, idxty, final, drowdim, dcoldimx, erowdimt, 1);

    //printf("final\n");
    for (i=0; i<drowdim; i++){
        printf("%.0f", final[i][0]);
        printf("\n");
	}

    for (i=0; i<erowdim; i++) {
            free(og[i]);
            free(x[i]);
            free(y[i]);
        }
    free(og);
    free(x);
    free(y);

    for (i=0; i<erowdimt; i++) {
            free(xt[i]);
            free(xtx[i]);
            free(id[i]);
            free(idxt[i]);
            free(idxty[i]);
        }
    free(xt);
    free(xtx);
    free(id);
    free(idxt);
    free(idxty);

    for(i=0; i<drowdim; i++) {
        free(ogd[i]);
        free(xd[i]);
        free(final[i]);
    }
    free(ogd);
    free(xd);
    free(final);

    return 0;

}
