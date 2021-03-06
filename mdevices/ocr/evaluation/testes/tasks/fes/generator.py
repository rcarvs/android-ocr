from sys import argv

script, filename = argv

file = open(filename,'w')

file.write("__kernel void identification( __global uint *trainData, __global uint *dataSize, __global uint *changes, __global uint *changesSize, __global uint *rotule, __global uint *result){ /*first pass is normalize the trainData passing the normalized version to rotule*/ uint crossingRotuleSize = 0; for(uint i=0;i<(changesSize[0]-1);i++){ if(changes[i] != changes[(i+1)]){ rotule[crossingRotuleSize] = changes[i]; crossingRotuleSize++; } } /*second pass is compare the rotule with the trainData to see if will match with any letter*/ uint i = 0; uint letter; uint quantity; while(i < dataSize[0]){ letter = trainData[i]; i++; quantity = trainData[i]; i++; uint j = 0; uint isFinded = 1; if(quantity == crossingRotuleSize){ while(j < quantity && j < crossingRotuleSize){ if(trainData[i+j] != rotule[j]){ isFinded = 0; break; } j++; } if(isFinded == 1){ result[0] = letter; break; } } i += quantity; } } ")
#__kernel void crossing(__global uint *letter,__global uint *width,__global uint *ccount){ uint id = get_global_id(0); uint changes = 0; for (uint teste = 0;teste < 1;teste++){ changes = 0; for(uint i=(id*width[0]);i < (id*width[0]+width[0]-1);i++){ if(letter[i] != letter[i+1]){ changes++; } } } ccount[id] = changes; } 
i = 1
size = 1000000
while i <= 1:
	file.write("__kernel void fe"+str(i)+"(__global uint *letter,__global uint *width,__global uint *ccount){ uint id = get_global_id(0); uint changes = 0; for (uint teste = 0;teste < "+str(size)+";teste++){ changes = 0; /*for(uint i=(id*width[0]);i < (id*width[0]+width[0]-1);i++){ if(letter[i] != letter[i+1]){ changes++; } }*/ } ccount[id] = width[0]; } ")
	#file.write("__kernel void test"+str(i)+"(__global uint *u,__global uint *v,__global uint *w,__global uint *x,__global uint *y,__global uint *z) { int gid = get_global_id(0); u[gid] = gid; v[gid] = gid;w[gid] = gid;x[gid] = gid;y[gid] = gid;z[gid] = gid;} ")
	i += 1	
file.write("__kernel void crossing(__global uint *letter,__global uint *width,__global uint *ccount){ uint id = get_global_id(0); uint changes = 0; for (uint teste = 0;teste < 1;teste++){ changes = 0; for(uint i=(id*width[0]);i < (id*width[0]+width[0]-1);i++){ if(letter[i] != letter[i+1]){ changes++; } } } ccount[id] = changes; } __kernel void blackandwhite(__global uchar4 *image,__global uint *r,__global uint *g,__global uint *b,__global uint *label,__global uint *checked) { int gid = get_global_id(0); uchar4 pixel = image[gid]; label[gid] = checked[gid] = 0; if((pixel.x+pixel.y+pixel.z) > 645){ r[gid] = g[gid] = b[gid] = pixel.x = pixel.y = pixel.z = 255; }else{ r[gid] = g[gid] = b[gid] = pixel.x = pixel.y = pixel.z = 0; } image[gid] = pixel; }")
