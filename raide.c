//32
#include"Model.hpp"
float raide_data[] = {0.297397,-0.025651,-0.996579,0,0,-1,
0.297397,-0.124783,-0.996579,0,0,-1,
0.198265,-0.124783,-0.996579,0,0,-1,
0.198265,-0.025651,-0.996579,0,0,-1,
0.297397,-0.025651,0.989084,0,-0,1,
0.198265,-0.025651,0.989084,0,-0,1,
0.198265,-0.124783,0.989084,0,-0,1,
0.297397,-0.124783,0.989084,0,-0,1,
0.297397,-0.025651,-0.996579,1,-0,0,
0.297397,-0.025651,0.989084,1,-0,0,
0.297397,-0.124783,0.989084,1,-0,0,
0.297397,-0.124783,-0.996579,1,-0,0,
0.297397,-0.124783,-0.996579,-0,-1,-0,
0.297397,-0.124783,0.989084,-0,-1,-0,
0.198265,-0.124783,0.989084,-0,-1,-0,
0.198265,-0.124783,-0.996579,-0,-1,-0,
0.198265,-0.124783,-0.996579,-1,0,0,
0.198265,-0.124783,0.989084,-1,0,0,
0.198265,-0.025651,0.989084,-1,0,0,
0.198265,-0.025651,-0.996579,-1,0,0,
0.297397,-0.025651,0.989084,0,1,0,
0.297397,-0.025651,-0.996579,0,1,0,
0.198265,-0.025651,-0.996579,0,1,0,
0.198265,-0.025651,0.989084,0,1,0,
-0.198265,-0.025651,0.989084,0,1,0,
-0.198265,-0.025651,-0.996579,0,1,0,
-0.297397,-0.025651,-0.996579,0,1,0,
-0.297398,-0.025651,0.989084,0,1,0,
-0.297398,-0.124783,-0.996579,-1,1e-06,-0,
-0.297398,-0.124783,0.989084,-1,1e-06,-0,
-0.297398,-0.025651,0.989084,-1,1e-06,-0,
-0.297397,-0.025651,-0.996579,-1,1e-06,-0,
-0.198265,-0.124783,-0.996579,-0,-1,-0,
-0.198265,-0.124783,0.989084,-0,-1,-0,
-0.297398,-0.124783,0.989084,-0,-1,-0,
-0.297398,-0.124783,-0.996579,-0,-1,-0,
-0.198265,-0.025651,-0.996579,1,-0,0,
-0.198265,-0.025651,0.989084,1,-0,0,
-0.198265,-0.124783,0.989084,1,-0,0,
-0.198265,-0.124783,-0.996579,1,-0,0,
-0.198265,-0.025651,0.989084,0,-0,1,
-0.297398,-0.025651,0.989084,0,-0,1,
-0.297398,-0.124783,0.989084,0,-0,1,
-0.198265,-0.124783,0.989084,0,-0,1,
-0.198265,-0.025651,-0.996579,0,0,-1,
-0.198265,-0.124783,-0.996579,0,0,-1,
-0.297398,-0.124783,-0.996579,0,0,-1,
-0.297397,-0.025651,-0.996579,0,0,-1,
-0.371747,-0.149566,-0.500638,0,0,-1,
-0.371747,-0.1,-0.500638,0,0,-1,
0.371747,-0.1,-0.500638,0,0,-1,
0.371747,-0.149566,-0.500638,0,0,-1,
0.371747,-0.1,-0.252807,0,-0,1,
-0.371747,-0.1,-0.252807,0,-0,1,
-0.371747,-0.149566,-0.252807,0,-0,1,
0.371747,-0.149566,-0.252807,0,-0,1,
0.371747,-0.1,-0.500638,1,-4e-06,0,
0.371747,-0.1,-0.252807,1,-4e-06,0,
0.371747,-0.149566,-0.252807,1,-4e-06,0,
0.371747,-0.149566,-0.500638,1,-4e-06,0,
0.371747,-0.149566,-0.500638,-0,-1,-0,
0.371747,-0.149566,-0.252807,-0,-1,-0,
-0.371747,-0.149566,-0.252807,-0,-1,-0,
-0.371747,-0.149566,-0.500638,-0,-1,-0,
-0.371747,-0.149566,-0.500638,-1,4e-06,-0,
-0.371747,-0.149566,-0.252807,-1,4e-06,-0,
-0.371747,-0.1,-0.252807,-1,4e-06,-0,
-0.371747,-0.1,-0.500638,-1,4e-06,-0,
-0.371747,-0.1,-0.500638,0,1,0,
-0.371747,-0.1,-0.252807,0,1,0,
0.371747,-0.1,-0.252807,0,1,0,
0.371747,-0.1,-0.500638,0,1,0,
-0.371747,-0.1,0.244168,0,1,0,
-0.371747,-0.1,0.492,0,1,0,
0.371747,-0.1,0.492,0,1,0,
0.371747,-0.1,0.244168,0,1,0,
-0.371747,-0.149566,0.244168,-1,4e-06,-0,
-0.371747,-0.149566,0.492,-1,4e-06,-0,
-0.371747,-0.1,0.492,-1,4e-06,-0,
-0.371747,-0.1,0.244168,-1,4e-06,-0,
0.371747,-0.149566,0.244168,-0,-1,-0,
0.371747,-0.149566,0.492,-0,-1,-0,
-0.371747,-0.149566,0.492,-0,-1,-0,
-0.371747,-0.149566,0.244168,-0,-1,-0,
0.371747,-0.1,0.244168,1,-4e-06,0,
0.371747,-0.1,0.492,1,-4e-06,0,
0.371747,-0.149566,0.492,1,-4e-06,0,
0.371747,-0.149566,0.244168,1,-4e-06,0,
0.371747,-0.1,0.492,0,-0,1,
-0.371747,-0.1,0.492,0,-0,1,
-0.371747,-0.149566,0.492,0,-0,1,
0.371747,-0.149566,0.492,0,-0,1,
-0.371747,-0.149566,0.244168,0,0,-1,
-0.371747,-0.1,0.244168,0,0,-1,
0.371747,-0.1,0.244168,0,0,-1,
0.371747,-0.149566,0.244168,0,0,-1,
0};
short raide_index[] = {0
,1,2,3,0,2,4,5,6,7,4,6,8,9,10,11,8
,10,12,13,14,15,12,14,16,17,18,19,16,18,20,21,22
,23,20,22,24,25,26,27,24,26,28,29,30,31,28,30,32
,33,34,35,32,34,36,37,38,39,36,38,40,41,42,43,40
,42,44,45,46,47,44,46,48,49,50,51,48,50,52,53,54
,55,52,54,56,57,58,59,56,58,60,61,62,63,60,62,64
,65,66,67,64,66,68,69,70,71,68,70,72,73,74,75,72
,74,76,77,78,79,76,78,80,81,82,83,80,82,84,85,86
,87,84,86,88,89,90,91,88,90,92,93,94,95,92,94};
 Model raide_model = {raide_data,raide_index,sizeof(raide_data)/(6*sizeof(float)),sizeof(raide_index)/sizeof(short)};