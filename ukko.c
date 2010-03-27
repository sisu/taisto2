//154
#include"Model.hpp"
float ukko_data[] = {-0.373657,-1,-1,-0.976577,-0.215166,-0,
-0.373657,-1,0.431964,-0.976577,-0.215166,-0,
-0.814311,1,1,-0.976577,-0.215166,-0,
-0.814311,1,-1,-0.976577,-0.215166,-0,
0.814311,1,1,0.976577,-0.215166,-0,
0.373657,-1,0.431964,0.976577,-0.215166,-0,
0.373657,-1,-1,0.976577,-0.215166,-0,
0.814311,1,-1,0.976577,-0.215166,-0,
0,-1,-1,0,-0,-1,
-0.373657,-1,-1,0,-0,-1,
-0.814311,1,-1,0,-0,-1,
0,1,-1,0,-0,-1,
0.814311,1,-1,0,-0,-1,
0.373657,-1,-1,0,-0,-1,
0,1,1,-0,-0.273212,0.961954,
-0.814311,1,1,-0,-0.273212,0.961954,
-0.373657,-1,0.431964,-0,-0.273212,0.961954,
0,-1,0.431964,-0,-0.273212,0.961954,
0.373657,-1,0.431964,0,-0.273212,0.961954,
0.814311,1,1,0,-0.273212,0.961954,
-0.373657,-1,-1,-1e-06,-1,-0,
0,-1,-1,-1e-06,-1,-0,
0,-1,0.431964,-1e-06,-1,-0,
-0.373657,-1,0.431964,-1e-06,-1,-0,
0,-1,0.431964,1e-06,-1,-0,
0,-1,-1,1e-06,-1,-0,
0.373657,-1,-1,1e-06,-1,-0,
0.373657,-1,0.431964,1e-06,-1,-0,
0,1,-1,0,1,0,
-0.814311,1,-1,0,1,0,
-0.814311,1,1,0,1,0,
0,1,1,0,1,0,
0.814311,1,1,-0,1,0,
0.814311,1,-1,-0,1,0,
-0.778263,0.415721,-0.470135,0,-0,-1,
-0.778263,0.784279,-0.470135,0,-0,-1,
-0.409705,0.784279,-0.470135,0,-0,-1,
-0.409705,0.415721,-0.470135,0,-0,-1,
0.409705,0.784279,-0.470135,0,-0,-1,
0.778263,0.784279,-0.470135,0,-0,-1,
0.778263,0.415721,-0.470135,0,-0,-1,
0.409705,0.415721,-0.470135,0,-0,-1,
-0.409705,0.784279,0.186117,0,-0,1,
-0.778263,0.784279,0.186117,0,-0,1,
-0.778263,0.415721,0.186117,0,-0,1,
-0.409705,0.415721,0.186117,0,-0,1,
0.778263,0.415721,0.186117,0,-0,1,
0.778263,0.784279,0.186117,0,-0,1,
0.409705,0.784279,0.186117,0,-0,1,
0.409705,0.415721,0.186117,0,-0,1,
-0.409705,0.784279,-0.470135,1,-0,0,
-0.409705,0.784279,0.186117,1,-0,0,
-0.409705,0.415721,0.186117,1,-0,0,
-0.409705,0.415721,-0.470135,1,-0,0,
0.409705,0.415721,0.186117,-1,-0,0,
0.409705,0.784279,0.186117,-1,-0,0,
0.409705,0.784279,-0.470135,-1,-0,0,
0.409705,0.415721,-0.470135,-1,-0,0,
-0.409705,0.415721,-0.470135,-0,-1,-0,
-0.409705,0.415721,0.186117,-0,-1,-0,
-0.778263,0.415721,0.186117,-0,-1,-0,
-0.778263,0.415721,-0.470135,-0,-1,-0,
0.778263,0.415721,0.186117,0,-1,-0,
0.409705,0.415721,0.186117,0,-1,-0,
0.409705,0.415721,-0.470135,0,-1,-0,
0.778263,0.415721,-0.470135,0,-1,-0,
-0.778263,0.784279,-0.470135,0,1,0,
-0.778263,0.784279,0.186117,0,1,0,
-0.409705,0.784279,0.186117,0,1,0,
-0.409705,0.784279,-0.470135,0,1,0,
0.409705,0.784279,0.186117,-0,1,0,
0.778263,0.784279,0.186117,-0,1,0,
0.778263,0.784279,-0.470135,-0,1,0,
0.409705,0.784279,-0.470135,-0,1,0,
-1.12443,0.784279,-0.470135,0,1,0,
-1.12443,0.784279,0.186117,0,1,0,
1.12443,0.784279,-0.470135,-0,1,0,
1.12443,0.784279,0.186117,-0,1,0,
-1.12443,0.415721,0.186117,-0,-1,0,
-1.12443,0.415721,-0.470135,-0,-1,0,
1.12443,0.415721,0.186117,0,-1,0,
1.12443,0.415721,-0.470135,0,-1,0,
-1.12443,0.415721,-0.470135,0,-0,-1,
-1.12443,0.784279,-0.470135,0,-0,-1,
1.12443,0.415721,-0.470135,-0,-0,-1,
1.12443,0.784279,-0.470135,-0,-0,-1,
-1.12443,0.415721,-0.470135,-1,0,0,
-1.12443,0.415721,0.186117,-1,0,0,
-1.12443,0.784279,0.186117,-1,0,0,
-1.12443,0.784279,-0.470135,-1,0,0,
1.12443,0.784279,0.186117,1,0,0,
1.12443,0.415721,0.186117,1,0,0,
1.12443,0.415721,-0.470135,1,0,0,
1.12443,0.784279,-0.470135,1,0,0,
-1.12443,0.415721,2.09672,-1,0,0,
-1.12443,0.784279,2.09672,-1,0,0,
1.12443,0.415721,2.09672,1,0,0,
1.12443,0.784279,2.09672,1,0,0,
-0.778263,0.415721,2.09672,-0,-1,0,
-1.12443,0.415721,2.09672,-0,-1,0,
0.778263,0.415721,2.09672,0,-1,0,
1.12443,0.415721,2.09672,0,-1,0,
-1.12443,0.784279,2.09672,0,1,0,
-0.778263,0.784279,2.09672,0,1,0,
1.12443,0.784279,2.09672,-0,1,0,
0.778263,0.784279,2.09672,-0,1,0,
-0.778263,0.415721,0.186117,1,-0,0,
-0.778263,0.784279,0.186117,1,-0,0,
-0.778263,0.784279,2.09672,1,-0,0,
-0.778263,0.415721,2.09672,1,-0,0,
0.778263,0.784279,2.09672,-1,-0,0,
0.778263,0.784279,0.186117,-1,-0,0,
0.778263,0.415721,0.186117,-1,-0,0,
0.778263,0.415721,2.09672,-1,-0,0,
-0.778263,0.415721,2.09672,0,-0,1,
-0.778263,0.784279,2.09672,0,-0,1,
-1.12443,0.784279,2.09672,0,-0,1,
-1.12443,0.415721,2.09672,0,-0,1,
1.12443,0.784279,2.09672,0,-0,1,
0.778263,0.784279,2.09672,0,-0,1,
0.778263,0.415721,2.09672,0,-0,1,
1.12443,0.415721,2.09672,0,-0,1,
-0.50976,-1.22061,-0.504631,0,-0,-1,
-0.31676,-0.779387,-0.504631,0,-0,-1,
-0.068535,-0.779387,-0.504631,0,-0,-1,
-0.193548,-1.22061,-0.504631,0,-0,-1,
0.068535,-0.779387,-0.504631,0,-0,-1,
0.31676,-0.779387,-0.504631,0,-0,-1,
0.50976,-1.22061,-0.504631,0,-0,-1,
0.193548,-1.22061,-0.504631,0,-0,-1,
-0.068534,-0.779387,0.077442,0,-0,1,
-0.31676,-0.779387,0.077442,0,-0,1,
-0.50976,-1.22061,0.077442,0,-0,1,
-0.193548,-1.22061,0.077442,0,-0,1,
0.50976,-1.22061,0.077442,0,-0,1,
0.31676,-0.779387,0.077442,0,-0,1,
0.068534,-0.779387,0.077442,0,-0,1,
0.193548,-1.22061,0.077442,0,-0,1,
-0.068535,-0.779387,-0.504631,0.962127,-0.272602,-0,
-0.068534,-0.779387,0.077442,0.962127,-0.272602,-0,
-0.193548,-1.22061,0.077442,0.962127,-0.272602,-0,
-0.193548,-1.22061,-0.504631,0.962127,-0.272602,-0,
0.193548,-1.22061,0.077442,-0.962127,-0.272602,-0,
0.068534,-0.779387,0.077442,-0.962127,-0.272602,-0,
0.068535,-0.779387,-0.504631,-0.962127,-0.272602,-0,
0.193548,-1.22061,-0.504631,-0.962127,-0.272602,-0,
-0.50976,-1.22061,-0.504631,-0.916185,0.400756,-0,
-0.50976,-1.22061,0.077442,-0.916185,0.400756,-0,
-0.31676,-0.779387,0.077442,-0.916185,0.400756,-0,
-0.31676,-0.779387,-0.504631,-0.916185,0.400756,-0,
0.31676,-0.779387,0.077442,0.916185,0.400756,-0,
0.50976,-1.22061,0.077442,0.916185,0.400756,-0,
0.50976,-1.22061,-0.504631,0.916185,0.400756,-0,
0.31676,-0.779387,-0.504631,0.916185,0.400756,-0,
-0.31676,-0.779387,-0.504631,0,1,0,
-0.31676,-0.779387,0.077442,0,1,0,
-0.068534,-0.779387,0.077442,0,1,0,
-0.068535,-0.779387,-0.504631,0,1,0,
0.068534,-0.779387,0.077442,-0,1,0,
0.31676,-0.779387,0.077442,-0,1,0,
0.31676,-0.779387,-0.504631,-0,1,0,
0.068535,-0.779387,-0.504631,-0,1,0,
-0.50976,-1.22061,0.077442,-1,0,-0,
-0.50976,-1.22061,-0.504631,-1,0,-0,
-0.50976,-2.08123,-0.504631,-1,0,-0,
-0.50976,-2.08123,0.077442,-1,0,-0,
0.50976,-2.08123,-0.504631,1,0,-0,
0.50976,-1.22061,-0.504631,1,0,-0,
0.50976,-1.22061,0.077442,1,0,-0,
0.50976,-2.08123,0.077442,1,0,-0,
-0.193548,-1.22061,-0.504631,1,0,0,
-0.193548,-1.22061,0.077442,1,0,0,
-0.193548,-2.08123,0.077442,1,0,0,
-0.193548,-2.08123,-0.504631,1,0,0,
0.193548,-2.08123,0.077442,-1,0,0,
0.193548,-1.22061,0.077442,-1,0,0,
0.193548,-1.22061,-0.504631,-1,0,0,
0.193548,-2.08123,-0.504631,-1,0,0,
-0.50976,-2.08123,0.077442,0,-0,1,
-0.193548,-2.08123,0.077442,0,-0,1,
0.50976,-2.08123,0.077442,0,-0,1,
0.193548,-2.08123,0.077442,0,-0,1,
-0.193548,-2.08123,-0.504631,0,-0,-1,
-0.50976,-2.08123,-0.504631,0,-0,-1,
0.193548,-2.08123,-0.504631,0,-0,-1,
0.50976,-2.08123,-0.504631,0,-0,-1,
-0.193548,-2.98481,-0.504631,0,-1,0,
-0.193548,-2.98481,0.077442,0,-1,0,
-0.50976,-2.98481,0.077442,0,-1,0,
-0.50976,-2.98481,-0.504631,0,-1,0,
0.50976,-2.98481,0.077442,0,-1,0,
0.193548,-2.98481,0.077442,0,-1,0,
0.193548,-2.98481,-0.504631,0,-1,0,
0.50976,-2.98481,-0.504631,0,-1,0,
-0.678263,0.914549,1.67381,1e-06,-1,1e-06,
-0.678263,0.914549,1.87381,1e-06,-1,1e-06,
-0.878263,0.914549,1.87381,1e-06,-1,1e-06,
-0.878263,0.914549,1.67381,1e-06,-1,1e-06,
0.878263,0.914549,1.87381,-1e-06,-1,1e-06,
0.678263,0.914549,1.87381,-1e-06,-1,1e-06,
0.678263,0.914549,1.67381,-1e-06,-1,1e-06,
0.878263,0.914549,1.67381,-1e-06,-1,1e-06,
-0.878263,0.285451,1.67381,-0,1,-0,
-0.878263,0.285451,1.87381,-0,1,-0,
-0.678263,0.285451,1.87381,-0,1,-0,
-0.678263,0.285451,1.67381,-0,1,-0,
0.678263,0.285451,1.87381,0,1,-0,
0.878263,0.285451,1.87381,0,1,-0,
0.878263,0.285451,1.67381,0,1,-0,
0.678263,0.285451,1.67381,0,1,-0,
-0.878263,0.914549,1.67381,0,-0,1,
-0.878263,0.66291,1.67381,0,-0,1,
-0.678263,0.66291,1.67381,0,-0,1,
-0.678263,0.914549,1.67381,0,-0,1,
0.678263,0.66291,1.67381,0,-0,1,
0.878263,0.66291,1.67381,0,-0,1,
0.878263,0.914549,1.67381,0,-0,1,
0.678263,0.914549,1.67381,0,-0,1,
-0.878263,0.285451,1.67381,0,-0,1,
-0.678263,0.285451,1.67381,0,-0,1,
0.678263,0.285451,1.67381,0,-0,1,
0.878263,0.285451,1.67381,0,-0,1,
-0.878263,0.285451,1.87381,0,-0,-1,
-0.878263,0.66291,1.87381,0,-0,-1,
-0.678263,0.66291,1.87381,0,-0,-1,
-0.678263,0.285451,1.87381,0,-0,-1,
0.678263,0.66291,1.87381,0,-0,-1,
0.878263,0.66291,1.87381,0,-0,-1,
0.878263,0.285451,1.87381,0,-0,-1,
0.678263,0.285451,1.87381,0,-0,-1,
-0.678263,0.285451,1.87381,-1,0,-0,
-0.678263,0.66291,1.87381,-1,0,-0,
-0.678263,0.66291,1.67381,-1,0,-0,
-0.678263,0.285451,1.67381,-1,0,-0,
0.678263,0.66291,1.67381,1,0,-0,
0.678263,0.66291,1.87381,1,0,-0,
0.678263,0.285451,1.87381,1,0,-0,
0.678263,0.285451,1.67381,1,0,-0,
-0.678263,0.66291,1.87381,-1,0,-1e-06,
-0.678263,0.914549,1.87381,-1,0,-1e-06,
-0.678263,0.914549,1.67381,-1,0,-1e-06,
-0.678263,0.66291,1.67381,-1,0,-1e-06,
0.678263,0.914549,1.67381,1,0,-1e-06,
0.678263,0.914549,1.87381,1,0,-1e-06,
0.678263,0.66291,1.87381,1,0,-1e-06,
0.678263,0.66291,1.67381,1,0,-1e-06,
-0.878263,0.914549,1.87381,1,0,0,
-0.878263,0.66291,1.87381,1,0,0,
-0.878263,0.66291,1.67381,1,0,0,
-0.878263,0.914549,1.67381,1,0,0,
0.878263,0.66291,1.67381,-1,0,0,
0.878263,0.66291,1.87381,-1,0,0,
0.878263,0.914549,1.87381,-1,0,0,
0.878263,0.914549,1.67381,-1,0,0,
-0.878263,0.285451,1.87381,1,0,-0,
-0.878263,0.285451,1.67381,1,0,-0,
0.878263,0.285451,1.67381,-1,-0,0,
0.878263,0.285451,1.87381,-1,-0,0,
-0.678263,0.66291,1.87381,-1e-06,1,0,
-0.878263,0.66291,1.87381,-1e-06,1,0,
-0.878263,0.66291,2.80885,-1e-06,1,0,
-0.678263,0.66291,2.80885,-1e-06,1,0,
0.878263,0.66291,2.80885,1e-06,1,0,
0.878263,0.66291,1.87381,1e-06,1,0,
0.678263,0.66291,1.87381,1e-06,1,0,
0.678263,0.66291,2.80885,1e-06,1,0,
-0.878263,0.914549,2.80885,1,0,0,
-0.878263,0.66291,2.80885,1,0,0,
0.878263,0.914549,2.80885,-1,0,0,
0.878263,0.66291,2.80885,-1,0,0,
-0.678263,0.914549,1.87381,-1,-0,0,
-0.678263,0.66291,2.80885,-1,-0,0,
-0.678263,0.914549,2.80885,-1,-0,0,
0.678263,0.66291,2.80885,1,0,-0,
0.678263,0.914549,1.87381,1,0,-0,
0.678263,0.914549,2.80885,1,0,-0,
-0.878263,0.914549,1.87381,1e-06,-1,0,
-0.678263,0.914549,1.87381,1e-06,-1,0,
-0.678263,0.914549,2.80885,1e-06,-1,0,
-0.878263,0.914549,2.80885,1e-06,-1,0,
0.678263,0.914549,2.80885,-1e-06,-1,0,
0.678263,0.914549,1.87381,-1e-06,-1,0,
0.878263,0.914549,1.87381,-1e-06,-1,0,
0.878263,0.914549,2.80885,-1e-06,-1,0,
-0.878263,0.66291,2.80885,0,-0,-1,
-0.878263,0.914549,2.80885,0,-0,-1,
-0.678263,0.914549,2.80885,0,-0,-1,
-0.678263,0.66291,2.80885,0,-0,-1,
0.678263,0.914549,2.80885,0,-0,-1,
0.878263,0.914549,2.80885,0,-0,-1,
0.878263,0.66291,2.80885,0,-0,-1,
0.678263,0.66291,2.80885,0,-0,-1,
-0.193548,-2.70019,-0.504631,0,-0,-1,
-0.50976,-2.70019,-0.504631,0,-0,-1,
0.50976,-2.70019,-0.504631,0,-0,-1,
0.193548,-2.70019,-0.504631,0,-0,-1,
-0.193548,-2.98481,-0.504631,0,-0,-1,
-0.50976,-2.98481,-0.504631,0,-0,-1,
0.50976,-2.98481,-0.504631,0,-0,-1,
0.193548,-2.98481,-0.504631,0,-0,-1,
-0.50976,-2.70019,0.077442,0,-0,1,
-0.193548,-2.70019,0.077442,0,-0,1,
0.193548,-2.70019,0.077442,0,-0,1,
0.50976,-2.70019,0.077442,0,-0,1,
-0.193548,-2.70019,0.077442,1,0,0,
-0.193548,-2.70019,-0.504631,1,0,0,
0.193548,-2.70019,-0.504631,-1,0,0,
0.193548,-2.70019,0.077442,-1,0,0,
-0.193548,-2.98481,0.077442,1,0,0,
-0.193548,-2.98481,-0.504631,1,0,0,
0.193548,-2.98481,-0.504631,-1,0,0,
0.193548,-2.98481,0.077442,-1,0,0,
-0.50976,-2.70019,-0.504631,-1,0,-0,
-0.50976,-2.70019,0.077442,-1,0,-0,
0.50976,-2.70019,0.077442,1,0,-0,
0.50976,-2.70019,-0.504631,1,0,-0,
-0.50976,-2.98481,-0.504631,-1,0,-0,
-0.50976,-2.98481,0.077442,-1,0,-0,
0.50976,-2.98481,0.077442,1,0,-0,
0.50976,-2.98481,-0.504631,1,0,-0,
-0.193548,-2.70019,0.077442,0,1,0,
-0.50976,-2.70019,0.077442,0,1,0,
-0.50976,-2.70019,0.698328,0,1,0,
-0.193548,-2.70019,0.698328,0,1,0,
0.50976,-2.70019,0.698328,0,1,0,
0.50976,-2.70019,0.077442,0,1,0,
0.193548,-2.70019,0.077442,0,1,0,
0.193548,-2.70019,0.698328,0,1,0,
-0.193548,-2.70019,0.698328,1,0,-0,
-0.193548,-2.98481,0.698328,1,0,-0,
0.193548,-2.70019,0.698328,-1,-0,0,
0.193548,-2.98481,0.698328,-1,-0,0,
-0.50976,-2.98481,0.698328,-1,-0,0,
-0.50976,-2.70019,0.698328,-1,-0,0,
0.50976,-2.98481,0.698328,1,0,-0,
0.50976,-2.70019,0.698328,1,0,-0,
-0.193548,-2.98481,0.698328,0,-1,0,
-0.50976,-2.98481,0.698328,0,-1,0,
0.193548,-2.98481,0.698328,0,-1,0,
0.50976,-2.98481,0.698328,0,-1,0,
-0.50976,-2.70019,0.698328,0,-0,1,
-0.50976,-2.98481,0.698328,0,-0,1,
-0.193548,-2.98481,0.698328,0,-0,1,
-0.193548,-2.70019,0.698328,0,-0,1,
0.193548,-2.98481,0.698328,0,-0,1,
0.50976,-2.98481,0.698328,0,-0,1,
0.50976,-2.70019,0.698328,0,-0,1,
0.193548,-2.70019,0.698328,0,-0,1,
-0.179768,0.915426,-0.1,-0,-0.274005,-0.961728,
-0.313696,1.98489,-0.4047,-0,-0.274005,-0.961728,
0.020232,1.98489,-0.4047,-0,-0.274005,-0.961728,
0.020232,0.915426,-0.1,-0,-0.274005,-0.961728,
-0.020232,1.98489,-0.4047,0,-0.274005,-0.961728,
0.313696,1.98489,-0.4047,0,-0.274005,-0.961728,
0.179768,0.915426,-0.1,0,-0.274005,-0.961728,
-0.020232,0.915426,-0.1,0,-0.274005,-0.961728,
0.020232,1.98489,0.439022,-1e-06,-0.302181,0.95325,
-0.313696,1.98489,0.439022,-1e-06,-0.302181,0.95325,
-0.179768,0.915426,0.1,-1e-06,-0.302181,0.95325,
0.020232,0.915426,0.1,-1e-06,-0.302181,0.95325,
0.179768,0.915426,0.1,1e-06,-0.302181,0.95325,
0.313696,1.98489,0.439022,1e-06,-0.302181,0.95325,
-0.020232,1.98489,0.439022,1e-06,-0.302181,0.95325,
-0.020232,0.915426,0.1,1e-06,-0.302181,0.95325,
0.020232,1.98489,-0.4047,1,-0,-0,
0.020232,1.98489,0.439022,1,-0,-0,
0.020232,0.915426,0.1,1,-0,-0,
0.020232,0.915426,-0.1,1,-0,-0,
-0.020232,0.915426,0.1,-1,-0,-0,
-0.020232,1.98489,0.439022,-1,-0,-0,
-0.020232,1.98489,-0.4047,-1,-0,-0,
-0.020232,0.915426,-0.1,-1,-0,-0,
0.020232,0.915426,-0.1,-3e-06,-1,-1e-06,
0.020232,0.915426,0.1,-3e-06,-1,-1e-06,
-0.179768,0.915426,0.1,-3e-06,-1,-1e-06,
-0.179768,0.915426,-0.1,-3e-06,-1,-1e-06,
0.179768,0.915426,0.1,3e-06,-1,-1e-06,
-0.020232,0.915426,0.1,3e-06,-1,-1e-06,
-0.020232,0.915426,-0.1,3e-06,-1,-1e-06,
0.179768,0.915426,-0.1,3e-06,-1,-1e-06,
-0.179768,0.915426,-0.1,-0.99225,-0.124258,0,
-0.179768,0.915426,0.1,-0.99225,-0.124258,0,
-0.313696,1.98489,0.439022,-0.99225,-0.124258,0,
-0.313696,1.98489,-0.4047,-0.99225,-0.124258,0,
0.313696,1.98489,0.439022,0.99225,-0.124258,0,
0.179768,0.915426,0.1,0.99225,-0.124258,0,
0.179768,0.915426,-0.1,0.99225,-0.124258,0,
0.313696,1.98489,-0.4047,0.99225,-0.124258,0,
-0.313696,1.98489,-0.4047,1e-06,1,0,
-0.313696,1.98489,0.439022,1e-06,1,0,
0.020232,1.98489,0.439022,1e-06,1,0,
0.020232,1.98489,-0.4047,1e-06,1,0,
-0.020232,1.98489,0.439022,-1e-06,1,0,
0.313696,1.98489,0.439022,-1e-06,1,0,
0.313696,1.98489,-0.4047,-1e-06,1,0,
-0.020232,1.98489,-0.4047,-1e-06,1,0,
-0.707382,1.64841,-0.707106,-0.209285,0.969636,-0.126518,
-0.954996,1.64841,-0.297503,-0.209285,0.969636,-0.126518,
0.013607,1.89289,-0.02606,-0.209285,0.969636,-0.126518,
-0.954996,1.64841,-0.297503,-0.240862,0.97045,-0.01457,
-0.983895,1.64841,0.180255,-0.240862,0.97045,-0.01457,
0.013607,1.89289,-0.02606,-0.240862,0.97045,-0.01457,
-0.983895,1.64841,0.180255,-0.217734,0.971076,0.097994,
-0.787458,1.64841,0.616718,-0.217734,0.971076,0.097994,
0.013607,1.89289,-0.02606,-0.217734,0.971076,0.097994,
-0.787458,1.64841,0.616718,-0.146459,0.971392,0.186941,
-0.410688,1.64841,0.911899,-0.146459,0.971392,0.186941,
0.013607,1.89289,-0.02606,-0.146459,0.971392,0.186941,
-0.410688,1.64841,0.911899,-0.042845,0.971341,0.233798,
0.060104,1.64841,0.998175,-0.042845,0.971341,0.233798,
0.013607,1.89289,-0.02606,-0.042845,0.971341,0.233798,
0.060104,1.64841,0.998175,0.071211,0.970931,0.228522,
0.517063,1.64841,0.855781,0.071211,0.970931,0.228522,
0.013607,1.89289,-0.02606,0.071211,0.970931,0.228522,
0.517063,1.64841,0.855781,0.171229,0.970238,0.171229,
0.855506,1.64841,0.517338,0.171229,0.970238,0.171229,
0.013607,1.89289,-0.02606,0.171229,0.970238,0.171229,
0.855506,1.64841,0.517338,0.234367,0.969401,0.073031,
0.997901,1.64841,0.060378,0.234367,0.969401,0.073031,
0.013607,1.89289,-0.02606,0.234367,0.969401,0.073031,
0.997901,1.64841,0.060378,0.244518,0.968609,-0.04481,
0.911625,1.64841,-0.410413,0.244518,0.968609,-0.04481,
0.013607,1.89289,-0.02606,0.244518,0.968609,-0.04481,
0.911625,1.64841,-0.410413,0.197367,0.968058,-0.154628,
0.616444,1.64841,-0.787183,0.197367,0.968058,-0.154628,
0.013607,1.89289,-0.02606,0.197367,0.968058,-0.154628,
0.616444,1.64841,-0.787183,0.103153,0.967899,-0.229197,
0.17998,1.64841,-0.983619,0.103153,0.967899,-0.229197,
0.013607,1.89289,-0.02606,0.103153,0.967899,-0.229197,
0.17998,1.64841,-0.983619,-0.015111,0.968176,-0.249816,
-0.297779,1.64841,-0.95472,-0.015111,0.968176,-0.249816,
0.013607,1.89289,-0.02606,-0.015111,0.968176,-0.249816,
-0.297779,1.64841,-0.95472,-0.128197,0.968811,-0.212064,
-0.707382,1.64841,-0.707106,-0.128197,0.968811,-0.212064,
0.013607,1.89289,-0.02606,-0.128197,0.968811,-0.212064,
0};
short ukko_index[] = {0
,1,2,3,0,2,4,5,6,7,4,6,8,9,10,11,8
,10,12,13,8,11,12,8,14,15,16,17,14,16,18,19,14
,17,18,14,20,21,22,23,20,22,24,25,26,27,24,26,28
,29,30,31,28,30,32,33,28,31,32,28,34,35,36,37,34
,36,38,39,40,41,38,40,42,43,44,45,42,44,46,47,48
,49,46,48,50,51,52,53,50,52,54,55,56,57,54,56,58
,59,60,61,58,60,62,63,64,65,62,64,66,67,68,69,66
,68,70,71,72,73,70,72,67,66,74,75,67,74,76,72,71
,77,76,71,61,60,78,79,61,78,80,62,65,81,80,65,35
,34,82,83,35,82,84,40,39,85,84,39,86,87,88,89,86
,88,90,91,92,93,90,92,88,87,94,95,88,94,96,91,90
,97,96,90,78,60,98,99,78,98,100,62,80,101,100,80,67
,75,102,103,67,102,104,77,71,105,104,71,106,107,108,109,106
,108,110,111,112,113,110,112,114,115,116,117,114,116,118,119,120
,121,118,120,122,123,124,125,122,124,126,127,128,129,126,128,130
,131,132,133,130,132,134,135,136,137,134,136,138,139,140,141,138
,140,142,143,144,145,142,144,146,147,148,149,146,148,150,151,152
,153,150,152,154,155,156,157,154,156,158,159,160,161,158,160,162
,163,164,165,162,164,166,167,168,169,166,168,170,171,172,173,170
,172,174,175,176,177,174,176,133,132,178,179,133,178,180,134,137
,181,180,137,122,125,182,183,122,182,184,129,128,185,184,128,186
,187,188,189,186,188,190,191,192,193,190,192,194,195,196,197,194
,196,198,199,200,201,198,200,202,203,204,205,202,204,206,207,208
,209,206,208,210,211,212,213,210,212,214,215,216,217,214,216,211
,218,219,212,211,219,220,221,215,214,220,215,222,223,224,225,222
,224,226,227,228,229,226,228,230,231,232,233,230,232,234,235,236
,237,234,236,238,239,240,241,238,240,242,243,244,245,242,244,246
,247,248,249,246,248,250,251,252,253,250,252,247,254,255,248,247
,255,256,257,251,250,256,251,258,259,260,261,258,260,262,263,264
,265,262,264,247,246,266,267,247,266,268,252,251,269,268,251,270
,231,271,272,270,271,273,235,274,275,273,274,276,277,278,279,276
,278,280,281,282,283,280,282,284,285,286,287,284,286,288,289,290
,291,288,290,182,292,293,183,182,293,294,295,184,185,294,184,292
,296,297,293,292,297,298,299,295,294,298,295,178,300,301,179,178
,301,302,303,180,181,302,180,172,304,305,173,172,305,306,307,174
,177,306,174,304,308,309,305,304,309,310,311,307,306,310,307,164
,312,313,165,164,313,314,315,166,169,314,166,312,316,317,313,312
,317,318,319,315,314,318,315,320,321,322,323,320,322,324,325,326
,327,324,326,308,304,328,329,308,328,330,307,311,331,330,311,313
,317,332,333,313,332,334,318,314,335,334,314,188,187,336,337,188
,336,338,191,190,339,338,190,340,341,342,343,340,342,344,345,346
,347,344,346,348,349,350,351,348,350,352,353,354,355,352,354,356
,357,358,359,356,358,360,361,362,363,360,362,364,365,366,367,364
,366,368,369,370,371,368,370,372,373,374,375,372,374,376,377,378
,379,376,378,380,381,382,383,380,382,384,385,386,387,384,386,388
,389,390,391,388,390,392,393,394,395,392,394,396,397,398,399,400
,401,402,403,404,405,406,407,408,409,410,411,412,413,414,415,416
,417,418,419,420,421,422,423,424,425,426,427,428,429,430,431,432
,433,434};
 Model ukko_model = {ukko_data,ukko_index,sizeof(ukko_data)/(6*sizeof(float)),sizeof(ukko_index)/sizeof(short)};
