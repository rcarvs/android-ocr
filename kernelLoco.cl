__kernel void labeling(__global uchar4 *image,__global uint *result,__global uint *label,__global uint *width,__global uint *height){                    
	int gid =get_global_id(0);						
	int size = width[0]*height[0];                                                 	
	uchar4 pixel = image[gid];
	
	int contList = 0;
	int connecteds[8];

	int top  = (((gid-width[0]) >= 0)?(gid-width[0]):-1);
	int topLeft = (((gid-width[0]-1)>=0 && convert_int(((gid-width[0])/width[0])) == convert_int(((gid-width[0]-1)/width[0])))?(gid-width[0]-1):-1);
	int topRight = (((gid-width[0]+1)<size && convert_int(((gid+width[0])/width[0])) == convert_int(((gid+width[0]+1)/width[0]))?(gid-width[0]+1):-1);
	int left = (((gid-1) >= 0 && convert_int((gid/width[0]))==convert_int(((gid-1)/width[0])))?(gid-1):-1);
	int right = (((gid+1) < size && convert_int((gid/width[0]))==convert_int(((gid+1)/width[0])))?(gid+1):-1);
	int down = (((gid+width[0]) < size)?(gid+width[0]):-1);
	int downLeft = (((gid+width[0]-1) < size && convert_int(((gid+width[0]-1)/width[0])) == convert_int(((gid+width[0])/width[0])))?(gid+width[0]-1):-1);
	int downRight = (((gid+width[0]+1) < size && convert_int(((gid+width[0]+1)/width[0])) == convert_int(((gid+width[0])/width[0])))?(gid+width[0]+1):-1);
	uchar4 pixelComp;
	if((pixel.x+pixel.y+pixel.z) == (255*3)){
		result[gid] = 0;
	}else{
		
		if(top != -1 && (pixel.x+pixel.y+pixel.z) == (pixelComp.x+pixelComp.y+pixelComp.z)){
			pixelComp = image[top];
			connecteds[contList] = top;
			contList++;
		}
		if(topLeft != -1 && (pixel.x+pixel.y+pixel.z) == (pixelComp.x+pixelComp.y+pixelComp.z)){
			pixelComp = image[topLeft];
			connecteds[contList] = topLeft;
			contList++;
		}		
		if(topRight != -1 && (pixel.x+pixel.y+pixel.z) == (pixelComp.x+pixelComp.y+pixelComp.z)){
			pixelComp = image[topRight];
			connecteds[contList] = topRight;
			contList++;
		}	
		if(down != -1 && (pixel.x+pixel.y+pixel.z) == (pixelComp.x+pixelComp.y+pixelComp.z)){
			pixelComp = image[down];
			connecteds[contList] = down;
			contList++;
		}	
		if(downLeft != -1 && (pixel.x+pixel.y+pixel.z) == (pixelComp.x+pixelComp.y+pixelComp.z)){
			pixelComp = image[downLeft];
			connecteds[contList] = downLeft;
			contList++;
		}		
		if(downRight != -1 && (pixel.x+pixel.y+pixel.z) == (pixelComp.x+pixelComp.y+pixelComp.z)){
			pixelComp = image[downRight];
			connecteds[contList] = downRight;
			contList++;
		}
		if(left != -1 && (pixel.x+pixel.y+pixel.z) == (pixelComp.x+pixelComp.y+pixelComp.z)){
			pixelComp = image[left];
			connecteds[contList] = left;
			contList++;
		}
		if(right != -1 && (pixel.x+pixel.y+pixel.z) == (pixelComp.x+pixelComp.y+pixelComp.z)){
			pixelComp = image[right];
			connecteds[contList] = right;
			contList++;
		}


		if(contList == 0){			
			result[gid] = 0;
		}else{
			int cont = 0;
			int lowestRotule = 0;
			while(cont < contList){
				if(lowestRotule == 0 && result[connecteds[cont]] != 0){
					lowestRotule = result[connecteds[cont]];
				}else if(result[connecteds[cont]] < lowestRotule){
					lowestRotule = result[connecteds[cont]];
				}
				cont++;
			}

			if(lowestRotule == 0){
				label[0] = label[0]+1;
				result[gid] = label[0];
			}else{
				result[gid] = lowestRotule;
			}

		}

		
	}
}