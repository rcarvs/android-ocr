"__kernel void identification(\n"
"	__global uint *trainData,\n"
"	__global uint *dataSize,\n"
"	__global uint *changes,\n"
"	__global uint *changesSize,\n"
"	__global uint *rotule,\n"
"	__global uint *result){\n"
"\n"
"	//first pass is normalize the trainData passing the normalized version to rotule\n"
"	uint crossingRotuleSize = 0;\n"
"	for(uint i=0;i<(changesSize[0]-1);i++){\n"
"		if(changes[i] != changes[(i+1)]){\n"
"			rotule[crossingRotuleSize] = changes[i];\n"
"			crossingRotuleSize++;\n"
"		}	\n"
"	}\n"
"	//second pass is compare the rotule with the trainData to see if will match with any letter\n"
"	uint i = 0;\n"
"	uint letter;\n"
"	uint quantity;\n"
"	while(i < dataSize[0]){\n"
"		letter = trainData[i];\n"
"		i++;\n"
"		quantity = trainData[i];\n"
"		i++;\n"
"		uint j = 0;		\n"
"		uint isFinded = 1;\n"
"		if(quantity == crossingRotuleSize){\n"
"			while(j < quantity && j < crossingRotuleSize){\n"
"				if(trainData[i+j] != rotule[j]){\n"
"					isFinded = 0;\n"
"					break;\n"
"				}\n"
"				j++;\n"
"			}\n"
"			if(isFinded == 1){\n"
"				result[0] = letter;\n"
"				break;\n"
"			}\n"
"		}		\n"
"		i += quantity;\n"
"		\n"
"	}\n"
"}\n"
__kernel void crossing(__global uint *letter,__global uint *width,__global uint *ccount){															
	uint id = get_global_id(0);														        														
	uint changes = 0;																        														
	for(uint i=(id*width[0]);i < (id*width[0]+width[0]-1);i++){						        														
		if(letter[i] != letter[i+1]){												        														
			changes++;																        														
		}																			        														
	}																				        														
	ccount[id] = changes;														            														
}																					        														
																					        														
__kernel void blackandwhite(__global uchar4 *image,__global uint *r,__global uint *g,__global uint *b,__global uint *label,__global uint *checked) {  
    int gid = get_global_id(0);                            						        														
    uchar4 pixel = image[gid];                             						        														
    label[gid] = checked[gid] = 0;																													
    if((pixel.x+pixel.y+pixel.z) > 645){                   						        														
		r[gid] = g[gid] = b[gid] = pixel.x = pixel.y = pixel.z = 255;                      															
    }else{                                                     					        														
    	r[gid] = g[gid] = b[gid] = pixel.x = pixel.y = pixel.z = 0;                        															
    }                                                          					        														
    image[gid] = pixel;                                        					        														
} 																																						
