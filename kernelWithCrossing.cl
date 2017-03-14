__kernel void identification(
	__global uint *trainData,
	__global uint *dataSize,
	__global uint *changes,
	__global uint *changesSize,
	__global uint *rotule,
	__global uint *result){

	//first pass is normalize the trainData passing the normalized version to rotule
	uint crossingRotuleSize = 0;
	for(uint i=0;i<(changesSize[0]-1);i++){
		if(changes[i] != changes[i+1]){
			rotule[crossingRotuleSize] = changes[i];
			crossingRotuleSize++;
		}	
	}
	//second pass is compare the rotule with the trainData to see if will match with any letter
	uint contRotules = 0;
	uint letterofTime = 0;
	for(uint i=0;i<dataSize[0];i++){
		if(contRotules == 0){
			letterofTime = trainData[i];
			i++;
			contRotules = trainData[i];
			if(contRotules == crossingRotuleSize){
				//if that to occur i need to compare each position of vector
				uint equals = 1;
				for(uint j=0;j<contRotules;j++){
					if(trainData[i+j] != rotule[j]){
						equals = 0;
						break;
					}
				}
				if(equals == 1){
					result[0] = letterofTime;
					break;
				}
				
			}
			i += contRotules;
			contRotules = 0;
		}
	}	
}
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
