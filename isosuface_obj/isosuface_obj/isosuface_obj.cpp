#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct {
	double x, y, z;
} XYZ;

typedef struct {
	XYZ p[3];
} TRIANGLE;

typedef struct {
	XYZ p[8];
	double val[8];
} GRIDCELL;

double length(double v[3])
{
	return (double)sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

double* normalize(double v[3])
{
	double l = v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
	l = 1 / (double)sqrt(l);
	v[0] *= l;
	v[1] *= l;
	v[2] *= l;
	return (v);
}

XYZ VertexInterp(double isolevel, XYZ p1, XYZ p2, double valp1, double valp2)
{
	double mu;
	XYZ p;
	if (fabs(isolevel - valp1) < 0.00001f)
		return (p1);
	if (fabs(isolevel - valp2) < 0.00001f)
		return (p2);
	if (fabs(valp1 - valp2) < 0.00001f)
		return (p1);
	mu = (isolevel - valp1) / (valp2 - valp1);
	p.x = p1.x + mu * (p2.x - p1.x);
	p.y = p1.y + mu * (p2.y - p1.y);
	p.z = p1.z + mu * (p2.z - p1.z);
	return (p);
}

int PolygoniseTri(GRIDCELL g, double iso, TRIANGLE* tri, int v0, int v1, int
	v2,
	int v3)
{
	int ntri = 0;
	int triindex;

	/*
 Determine which of the 16 cases is found for which vertices
 are above or below the isosurface
 */
	triindex = 0;
	if (g.val[v0] < iso)
		triindex |= 1;
	if (g.val[v1] < iso)
		triindex |= 2;
	if (g.val[v2] < iso)
		triindex |= 4;
	if (g.val[v3] < iso)
		triindex |= 8;
	/* Find the vertices of the triangles for each case */
	switch (triindex) {
	case 0x00:
	case 0x0F:
		break;
	case 0x0E:
	case 0x01:
		tri[0].p[0] = VertexInterp(iso, g.p[v0], g.p[v1], g.val[v0], g.val[v1]);
		tri[0].p[1] = VertexInterp(iso, g.p[v0], g.p[v2], g.val[v0], g.val[v2]);
		tri[0].p[2] = VertexInterp(iso, g.p[v0], g.p[v3], g.val[v0], g.val[v3]);
		ntri++;
		break;
	case 0x0D:
	case 0x02:
		tri[0].p[0] = VertexInterp(iso, g.p[v1], g.p[v0], g.val[v1], g.val[v0]);
		tri[0].p[1] = VertexInterp(iso, g.p[v1], g.p[v3], g.val[v1], g.val[v3]);
		tri[0].p[2] = VertexInterp(iso, g.p[v1], g.p[v2], g.val[v1], g.val[v2]);
		ntri++;
		break;
	case 0x0C:
	case 0x03:
		tri[0].p[0] = VertexInterp(iso, g.p[v0], g.p[v3], g.val[v0], g.val[v3]);
		tri[0].p[1] = VertexInterp(iso, g.p[v0], g.p[v2], g.val[v0], g.val[v2]);
		tri[0].p[2] = VertexInterp(iso, g.p[v1], g.p[v3], g.val[v1], g.val[v3]);
		ntri++;
		tri[1].p[0] = tri[0].p[2];
		tri[1].p[1] = VertexInterp(iso, g.p[v1], g.p[v2], g.val[v1], g.val[v2]);
		tri[1].p[2] = tri[0].p[1];
		ntri++;
		break;
	case 0x0B:
	case 0x04:
		tri[0].p[0] = VertexInterp(iso, g.p[v2], g.p[v0], g.val[v2], g.val[v0]);
		tri[0].p[1] = VertexInterp(iso, g.p[v2], g.p[v1], g.val[v2], g.val[v1]);
		tri[0].p[2] = VertexInterp(iso, g.p[v2], g.p[v3], g.val[v2], g.val[v3]);
		ntri++;
		break;
	case 0x0A:
	case 0x05:
		tri[0].p[0] = VertexInterp(iso, g.p[v0], g.p[v1], g.val[v0], g.val[v1]);
		tri[0].p[1] = VertexInterp(iso, g.p[v2], g.p[v3], g.val[v2], g.val[v3]);
		tri[0].p[2] = VertexInterp(iso, g.p[v0], g.p[v3], g.val[v0], g.val[v3]);
		ntri++;
		tri[1].p[0] = tri[0].p[0];
		tri[1].p[1] = VertexInterp(iso, g.p[v1], g.p[v2], g.val[v1], g.val[v2]);
		tri[1].p[2] = tri[0].p[1];

		ntri++;
		break;
	case 0x09:
	case 0x06:
		tri[0].p[0] = VertexInterp(iso, g.p[v0], g.p[v1], g.val[v0], g.val[v1]);
		tri[0].p[1] = VertexInterp(iso, g.p[v1], g.p[v3], g.val[v1], g.val[v3]);
		tri[0].p[2] = VertexInterp(iso, g.p[v2], g.p[v3], g.val[v2], g.val[v3]);
		ntri++;
		tri[1].p[0] = tri[0].p[0];
		tri[1].p[1] = VertexInterp(iso, g.p[v0], g.p[v2], g.val[v0], g.val[v2]);
		tri[1].p[2] = tri[0].p[2];
		ntri++;
		break;
	case 0x07:
	case 0x08:
		tri[0].p[0] = VertexInterp(iso, g.p[v3], g.p[v0], g.val[v3], g.val[v0]);
		tri[0].p[1] = VertexInterp(iso, g.p[v3], g.p[v2], g.val[v3], g.val[v2]);
		tri[0].p[2] = VertexInterp(iso, g.p[v3], g.p[v1], g.val[v3], g.val[v1]);
		ntri++;
		break;
	}
	return (ntri);
}
#define NX 32
#define NY 32
#define NZ 32
int main(int argc, char** argv)
{
	int i, j, k, l, n, c;
	unsigned char* data1 = NULL;
	unsigned char(*data)[NY][NZ];
	int isolevel = 128;
	int themax, themin;
	GRIDCELL grid;
	TRIANGLE triangles[2];
	TRIANGLE* tri = NULL;
	int ntri = 0;
	FILE* fptr;
	double *VertexNormals;
	int counter = 0;
	if (argc < 2) {
		fprintf(stderr, "Usage: %s [options] volumetricfilename\n", argv[0]);
		fprintf(stderr, "Options\n");
		fprintf(stderr, " -i isosurface value\n");

		exit(-1);
	}
	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-i") == 0)
			isolevel = atof(argv[i + 1]);
	}

	data1 = (unsigned char *)malloc(NX * NY * NZ * sizeof(unsigned char));
	// Open and read the raw data
	fprintf(stderr, "Reading data ...\n");
	if ((fptr = fopen(argv[argc - 1], "rb")) == NULL) {
		fprintf(stderr, "File open failed\n");
		exit(-1);
	}
	fread(data1, sizeof(unsigned char), NX * NY * NZ, fptr);
	data = (unsigned char(*)[NY][NZ])data1;

	themin = themax = data[0][0][0];
	for (i = 0; i < NX; i++) // Find the Range of Scalar Values
		for (j = 0; j < NY; j++)
			for (k = 0; k < NZ; k++) {
				if (themin > data[i][j][k])
					themin = data[i][j][k];
				if (themax < data[i][j][k])
					themax = data[i][j][k];
			}
	fclose(fptr);
	fprintf(stderr, "Data range: Min %d -> Max :%d\n", themin, themax);
	// Polygonise the grid
	fprintf(stderr, "Polygonising data ...\n");
	for (i = 0; i < NX - 1; i++) {
		if (i % (NX / 10) == 0)
			printf(" Slice %d of %d\n", i, NX);
		for (j = 0; j < NY - 1; j++) {
			for (k = 0; k < NZ - 1; k++) {
				grid.p[0].x = i;
				grid.p[0].y = j;
				grid.p[0].z = k;
				grid.val[0] = data[i][j][k]; //Vertex 0 of the cube
				grid.p[1].x = i + 1;
				grid.p[1].y = j;
				grid.p[1].z = k;
				grid.val[1] = data[i + 1][j][k]; //Vertex 1 of the cube
				grid.p[2].x = i + 1;
				grid.p[2].y = j + 1;
				grid.p[2].z = k;
				grid.val[2] = data[i + 1][j + 1][k]; //Vertex 2 of the cube
				grid.p[3].x = i;
				grid.p[3].y = j + 1;
				grid.p[3].z = k;
				grid.val[3] = data[i][j + 1][k]; //Vertex 3 of the cube
				grid.p[4].x = i;
				grid.p[4].y = j;
				grid.p[4].z = k + 1;
				grid.val[4] = data[i][j][k + 1]; //Vertex 4 of the cube
				grid.p[5].x = i + 1;
				grid.p[5].y = j;
				grid.p[5].z = k + 1;
				grid.val[5] = data[i + 1][j][k + 1]; //Vertex 5 of the cube
				grid.p[6].x = i + 1;
				grid.p[6].y = j + 1;
				grid.p[6].z = k + 1;
				grid.val[6] = data[i + 1][j + 1][k + 1]; //Vertex 6 of the cube
				grid.p[7].x = i;
				grid.p[7].y = j + 1;
				grid.p[7].z = k + 1;
				grid.val[7] = data[i][j + 1][k + 1]; //Vertex 7 of the cube
				n = PolygoniseTri(grid, isolevel, triangles, 0, 2, 3, 7); //Tet 1
				tri = (TRIANGLE *)realloc(tri, (ntri + n) * sizeof(TRIANGLE));
				for (l = 0; l < n; l++)
					tri[ntri + l] = triangles[l];
				ntri += n;
				n = PolygoniseTri(grid, isolevel, triangles, 0, 2, 6, 7); //Tet 2
				tri = (TRIANGLE *)realloc(tri, (ntri + n) * sizeof(TRIANGLE));
				for (l = 0; l < n; l++)
					tri[ntri + l] = triangles[l];
				ntri += n;
				n = PolygoniseTri(grid, isolevel, triangles, 0, 4, 6, 7); //Tet3
				tri = (TRIANGLE *)realloc(tri, (ntri + n) * sizeof(TRIANGLE));
				for (l = 0; l < n; l++)
					tri[ntri + l] = triangles[l];
				ntri += n;
				n = PolygoniseTri(grid, isolevel, triangles, 0, 6, 1, 2); //Tet 4
				tri = (TRIANGLE *)realloc(tri, (ntri + n) * sizeof(TRIANGLE));
				for (l = 0; l < n; l++)
					tri[ntri + l] = triangles[l];
				ntri += n;
				n = PolygoniseTri(grid, isolevel, triangles, 0, 6, 1, 4); //Tet5
				tri = (TRIANGLE *)realloc(tri, (ntri + n) * sizeof(TRIANGLE));
				for (l = 0; l < n; l++)
					tri[ntri + l] = triangles[l];
				ntri += n;
				n = PolygoniseTri(grid, isolevel, triangles, 5, 6, 1, 4); //Tet 6
				tri = (TRIANGLE *)realloc(tri, (ntri + n) * sizeof(TRIANGLE));
				for (l = 0; l < n; l++)
					tri[ntri + l] = triangles[l];
				ntri += n;
			}
		}
	}
	fprintf(stderr, "Total of %d triangles\n", ntri);
	fprintf(stderr, "Writing triangles ...\n");
	if ((fptr = fopen("output.obj", "w")) == NULL) {
		fprintf(stderr, "Failed to open output file\n");
		exit(-1);
	}
	for (i = 0; i < ntri; i++) {

		for (k = 0; k < 3; k++) {
			fprintf(fptr, "v ");
			fprintf(fptr, "%g %g %g \n", tri[i].p[k].x, tri[i].p[k].y, tri[i].p[k].z);
			//double *VertexNormal = normalize(tri[i].p[k].x, tri[i].p[k].y, tri[i].p[k].z);
			VertexNormals = normalize((double *)&tri[i].p[k]);
			fprintf(fptr, "vn ");
			fprintf(fptr, "%g %g %g \n", VertexNormals[0], VertexNormals[1], VertexNormals[2]);
		}
	}

	int I = 0; //TODO: wtf !?
	for (i = 1; i <= ntri; i = i + 3) {
		fprintf(fptr, "f ");
		fprintf(fptr, "%d/%d %d/%d %d/%d \n ", i, I, i + 1, i + 1, i + 2, i + 2);
	}
	fclose(fptr);
	exit(0);
}