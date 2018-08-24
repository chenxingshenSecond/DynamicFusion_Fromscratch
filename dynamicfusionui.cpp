#include "Draw_C.h"
#include "glm/glm.hpp" // glm 
#include "Glsl_loader.h"
//   //  
Draw_multiple_obj::Draw_multiple_obj() // 
{  //  
	WIN_DOW_SCALE    =  1000  ; // 
	Window_size      =  1.0f  ; // 
	window_w         =  100   ; // 
	window_h         =  100   ; // 
	Full_Screen_Flag = false; // 
	//  Denotion_texture_remapping Poster上面的纹理重新映射   //  
	int Denotion_remapping_0[][2] = { {1,1},{0,2},{1,0},{0,0},
	                                  {1,3},{1,6},{0,7},{0,5},
									  {1,5},{0,3},{1,4} } ;  //   

															 //
	Flag__GRASP =  0 ; // 
	Organ_num   = -1 ; //  
	/*
	//int Denotion_remapping_0[][2] = { { 0,6 },{ 0,5 },{ 0,4 },
	//{ 0,3 },{ 0,2 },{ 0,1 },
	//{ 0,0 },{ 1,6 },{ 1,5 },
	//{ 1,4 },{ 1,3 },{ 1,2 },
	//{ 1,2 },{ 1,0 },{ 1,1 } };  //   																																									 // int Denotion_texture_remapping_0[][2] = { { 0,6 },{ 0,5 },{ 0,4 },{ 0,3 },{ 0,2 },{ 0,1 } ,{ 0, 0 },{ 1,6 },{ 1 ,5 } ,{ 1 ,4 } ,{ 1 , 3 },{ 1,2 },{ 1,2 },{ 1,0 } ,{ 1,1 } };  // 
	// int Denotion_texture_remapping_0 [][2] = {  { 0,6 },{ 0,5 },{ 0,4 },{ 0,3 },{ 0,2 },{ 0,1 } ,{ 0, 0},{ 1,6 },{ 1 ,5 } ,{1 ,4 } , { 1 , 3 },{ 1,2 },{ 1,2 },{ 1,0 } ,{ 1,1 } } ;  //  
	//Denotion_texture_remapping = (int**) malloc( 14 * sizeof(int*) ) ; //  
	//for (int i= 0;i<14 ;i++ ) // 
	//{ // 
	//	Denotion_texture_remapping[i]    = (int*) malloc( 2 * sizeof( int ) ) ;
	//	Denotion_texture_remapping[i][0] = Denotion_texture_remapping_0[i][0] ;
	//	Denotion_texture_remapping[i][1] = Denotion_texture_remapping_0[i][1] ; 
	//} // 
	*/ //    
	Denotion_texture_remapping = (int**)malloc(11 * sizeof(int*)); //  
	for (int i = 0; i<11; i++) // 
	{  	Denotion_texture_remapping[i] = (int*)malloc(2 * sizeof(int));
		Denotion_texture_remapping[i][0] = Denotion_remapping_0[i][0];
		Denotion_texture_remapping[i][1] = Denotion_remapping_0[i][1]; 
	} //     // 
	m_baiscobj     = new Obj_control  () ; //   控制运动相关和基本处理 
	fps_obj        = new time_fps     () ; //   计时           // 
	mult_camera    = new Multiple_View() ; //   默认初始化     //  
	// 
	glsl_obj_softbody       = new GLSL_Loader("Data//Liver_resized_to_2018__SB_Removed.obj") ; //  
	glsl_obj_ball_marker    = new GLSL_Loader("Data//earth_tri_1000.obj" )   ; //   
	glsl_obj_cutter         = new GLSL_Loader("Data//cutter1_stylus-.obj")   ; //
    // glsl_obj_cutter = new GLSL_Loader("Data//Palm_Stylus.obj");  // 
	glsl_obj_poster         = new GLSL_Loader("Data//Poster.obj")  ;  //      
	// 
	txt_obj        = new txt_Draw()      ; //
	screen_obj     = new SCREEN_SHOT()   ; // 
	counter        =   0  ;  // 
	glsl_Flag      =   1  ;  //
	menu_display   =  'l' ;  // 
	// 
	glsl_obj_ball_marker->Max_Size   = mult_camera->WIDTH * mult_camera->dx  / 32.0   ;   //  // 相对的尺寸 // 
	glsl_obj_softbody->Max_Size      = mult_camera->WIDTH * mult_camera->dx  / 2.8   ;   //  // 相对的尺寸 // 
	glsl_obj_cutter->Max_Size        = mult_camera->WIDTH * mult_camera->dx  / 3.0   ;   //  // 相对的尺寸 //
	glsl_obj_poster->Max_Size        = mult_camera->WIDTH * mult_camera->dx  / 5.0   ;   //  // 相对的尺寸 //
	//  
	Face_Limit_Rectangle = (int **) malloc (  mult_camera->MLA_NUMBDER_RECTANGLE  * sizeof( int*) ) ; // // 
	for(int i = 0; i < mult_camera->MLA_NUMBDER_RECTANGLE ; i ++) { Face_Limit_Rectangle[i] = (int *) malloc(   4 * sizeof(int) ) ; }  
	Face_Limit_Hexagon   = new int * [ mult_camera->MLA_NUMBDER_HEXAGON ] ; //      // 
	for(int i = 0; i < mult_camera->MLA_NUMBDER_HEXAGON; i ++) {Face_Limit_Hexagon[i]=new int[ 4 ]; } //  
	Load_csv( )  ; //    
	// screen_obj->If_SaveImage = true; //  
	iteration_of_display = 0; 
}
//
void Draw_multiple_obj::utility_show_glm_mat4( string src_name , glm::mat4 src)
{
	std::cout<<src_name<<" is : "<<endl ;
	for (int i=0 ; i<4 ; i++)
	{
		for (int j=0 ; j<4 ; j++)
		{   std::cout<<"\t"<<src[j][i]; }
		std::cout<<endl; 
	}
	std::cout<<endl; 
}
 //  
void Draw_multiple_obj::Triangle_Primitive_Determine ( Octree* tmp  ) 
{  	// glsl_obj_softbody->mesh.face_num ;  //  
	float3 xyz           ; //   
	float3 xyz_in_world  ; //  
	int    index_min[2]  ; // 
	int    index_max[2]  ; //  
	// 
	int index_tmp    ; // 
	int index_tmp_x  ; // 
	int index_tmp_y  ; // 
	//  
	if( mult_camera->IF_Square ) 
	{ // indexData 是从1开始还是从 0 
		#pragma omp parallel for
		for ( int i = 0 ; i < mult_camera->LENS_NUM_X ; i++ )
		{
			#pragma omp parallel for
			for ( int j = 0 ; j<mult_camera->LENS_NUM_Y ;j++ )	
			{  
				Face_Limit_Rectangle[ i * mult_camera->LENS_NUM_Y + j ][0] = glsl_obj_softbody->Face_Num  - 1  ;  // min //  
				Face_Limit_Rectangle[ i * mult_camera->LENS_NUM_Y + j ][1] = 0 ;  // max // 
			} 
		} 
	} 
	// 
	float Pitch_half_GAP = mult_camera->PITCH / 2.0 / mult_camera->GAP ;
	// 
	if( mult_camera->IF_Square ) 
	{ // 
		#pragma omp parallel for
		for (int iter = 0; iter < glsl_obj_softbody->Face_Num-20; iter+= 20  ) // 
		{
			// cout << "iter = "<<iter << endl;
			// cout << glsl_obj_softbody->indexData[3 * iter + 1]  << endl;
			index_tmp = glsl_obj_softbody->indexData[ 3 * iter + 1 ];  //  // xyz  = glsl_obj_softbody->mesh.verts[ index_tmp ]  ;   
			xyz.x = glsl_obj_softbody->pvertex[index_tmp].x  ; 
			xyz.y = glsl_obj_softbody->pvertex[index_tmp].y  ; 
			xyz.z = glsl_obj_softbody->pvertex[index_tmp].z  ; 
			//// new 函数  //  // 
			xyz_in_world.x = m_baiscobj->Transform_Matrix_Mouse[0 + 0] * xyz.x + m_baiscobj->Transform_Matrix_Mouse[0 + 4] * xyz.y + m_baiscobj->Transform_Matrix_Mouse[8 + 0] * xyz.z ;  //  
			xyz_in_world.y = m_baiscobj->Transform_Matrix_Mouse[1 + 0] * xyz.x + m_baiscobj->Transform_Matrix_Mouse[1 + 4] * xyz.y + m_baiscobj->Transform_Matrix_Mouse[8 + 1] * xyz.z ;  // 
			xyz_in_world.z = m_baiscobj->Transform_Matrix_Mouse[2 + 0] * xyz.x + m_baiscobj->Transform_Matrix_Mouse[2 + 4] * xyz.y + m_baiscobj->Transform_Matrix_Mouse[8 + 2] * xyz.z + 100 ;  //  + 100 那么速度下降很多 
			// 
			if ( xyz_in_world.z > 0 ) //   //  
			{
				index_tmp = xyz_in_world.z * Pitch_half_GAP;   // 
				//
				index_min[0] = max(         0               , int((xyz_in_world.x - index_tmp + mult_camera->WIDTH  * mult_camera->dx / 2) / mult_camera->PITCH));  ///
				index_max[0] = min( mult_camera->LENS_NUM_X , int((xyz_in_world.x + index_tmp + mult_camera->WIDTH  * mult_camera->dx / 2) / mult_camera->PITCH));  ///
				// 
				index_min[1] = max(        0                , int((xyz_in_world.y - index_tmp + mult_camera->HEIGHT * mult_camera->dx / 2) / mult_camera->PITCH));  ///
				index_max[1] = min( mult_camera->LENS_NUM_Y , int((xyz_in_world.y + index_tmp + mult_camera->HEIGHT * mult_camera->dx / 2) / mult_camera->PITCH));  ///

			    #pragma omp parallel for
				for (int i = index_min[0]; i < index_max[0]; i++ )
				{
                    #pragma omp parallel for    // 
					for (int j = index_min[1]; j < index_max[1]; j++ )
					{ 
						Face_Limit_Rectangle[ i * mult_camera->LENS_NUM_Y + j ][0] = min( Face_Limit_Rectangle[ i * mult_camera->LENS_NUM_Y + j ][0] , iter ); 
						Face_Limit_Rectangle[ i * mult_camera->LENS_NUM_Y + j ][1] = max( Face_Limit_Rectangle[ i * mult_camera->LENS_NUM_Y + j ][1] , iter );   
					}
				}
			}
		}
	}

}

//   可能要根据镜子的新位置计算才准确   //  
glm::mat4   Draw_multiple_obj::Get_mat4_from_float_Fcn ( GLfloat * src ) 
{
	glm::mat4 result ; 
	for (int i=0 ; i<4 ; i++)
	{
		for (int j=0 ; j<4 ; j++)
		{ // 
			result[i][j] = src[ i*4+j ] ; // 
		} //
	}
	return result ;  
}
void Draw_multiple_obj::Projection_Setup()
{ 
	glEnable   ( GL_DEPTH_TEST )  ;    //  
	glDepthFunc(  GL_LESS      )  ;    //  
	glClearDepth   ( 1 )          ;    //   
	glMatrixMode(GL_PROJECTION)   ;   glPushMatrix() ; //
	glLoadIdentity()  ;   
	glViewport(0,0,mult_camera->WIDTH,mult_camera->HEIGHT );   // Meffy：渲染视口√
	// 
	// 
	GLfloat Window_size   =  mult_camera->WIDTH  * mult_camera->dx / 2.0 ;   // 
	GLfloat Window_size_y =  mult_camera->HEIGHT * mult_camera->dx / 2.0 ;   //   
	// 
	// 
	glOrtho  ( - Window_size , Window_size , -Window_size_y  , Window_size_y  , -600 , 600 ) ; // 
	GLdouble eyex   = 0  , eyey    = 0 , eyez    = 300 , centerx= 0  , centery = 0 , centerz = 0   ,  	upx    = 0  , upy     = 1 , upz     = 0   ; //  
	gluLookAt( eyex, eyey, eyez, centerx, centery, centerz, upx, upy, upz);   //  注意的 //   // 
	// 
	// 
	glGetFloatv( GL_PROJECTION_MATRIX , glsl_obj_softbody->project_matrix      ) ;   // Meffy: 获取模型视图矩阵，使物体正向 
	glGetFloatv( GL_PROJECTION_MATRIX , glsl_obj_ball_marker->project_matrix   ) ;   // Meffy: 获取模型视图矩阵，使物体正向 
	glGetFloatv( GL_PROJECTION_MATRIX , glsl_obj_cutter->project_matrix        ) ;   // Meffy: 获取模型视图矩阵，使物体正向  
	glGetFloatv( GL_PROJECTION_MATRIX , glsl_obj_poster->project_matrix        ) ;   // Meffy: 获取模型视图矩阵，使物体正向  
	// 
	glMatrixMode(GL_PROJECTION);  	glPopMatrix(); //
	// 
	memcpy ( glsl_obj_ball_marker->model_view        , m_baiscobj->Transform_Matrix_Leap  , 16 * sizeof(float) ) ;  //  
	memcpy ( glsl_obj_softbody->model_view           , m_baiscobj->Transform_Matrix_Mouse , 16 * sizeof(float) ) ;  //   
	memcpy ( glsl_obj_cutter->model_view             , m_baiscobj->Transform_Matrix_Leap  , 16 * sizeof(float) ) ;  // 
	glm::mat4  tmp ; 
	memcpy ( &tmp[0][0],m_baiscobj->Transform_Matrix_Leap,16 * sizeof(float) ) ;  //       
	//   *********************************   // 

	Flag__GRASP = 0;  Organ_num = -1;   //     
	if (  m_baiscobj->listener->HandNum > 0  )
	{ // 
		glsl_obj_softbody->Probe_Hand_Origin   
		= float3( glsl_obj_ball_marker->model_view[12] - glsl_obj_ball_marker->model_view[12 - 4] * 80, // cgw //
				  glsl_obj_ball_marker->model_view[13] - glsl_obj_ball_marker->model_view[13 - 4] * 80, // cgw // 
				  glsl_obj_ball_marker->model_view[14] - glsl_obj_ball_marker->model_view[14 - 4] * 80  ); //    
		// 
		glsl_obj_softbody->Probe_Hand_direction = float3(-m_baiscobj->Transform_Matrix_Leap[8], -m_baiscobj->Transform_Matrix_Leap[9], -m_baiscobj->Transform_Matrix_Leap[10]); //   
		Grasped_Vertex_ID   =  glsl_obj_softbody->Intersect_leap_probe(  m_baiscobj->Transform_Matrix_Mouse  ); // 
		if ( Grasped_Vertex_ID > 0 )  //         system( "pause" ) ;    //    
		{  	
			glsl_obj_softbody->temporary_count_ = 0 ; // 
			Flag__GRASP                         = 1 ; //  
		} // 
	}
}
// 
int Draw_multiple_obj::Organ_Indentification( float x , float y ,float z  ) 
{
	int count = strArray.size() - 1     ;  // 
	cout << "count = " << count << endl ;  // count cgw // 
	float min_dist    =  10000 ; // 
	int   index       =  -1    ; //
	float scale  = glsl_obj_softbody->Max_Size / 30.06629;
	for (int i = 0 ; i < count ; i++ ) // 
	{
		float tmp = (x - Organ[i][0] * scale) * (x - Organ[i][0] * scale) + (y - Organ[i][1] * scale) * (y - Organ[i][1] * scale) + (z - Organ[i][2] * scale) * (z - Organ[i][2] * scale);
		if (tmp < min_dist)
		{
			index = i;
			min_dist = tmp;
		}
	} // 
	return index;
}
// 
int Draw_multiple_obj::Organ_Indentification( int vertex_id ) // 
{ 
	int count = strArray.size() - 1  ;  //   
	return Organ[vertex_id][0];
}


//
// 	vec_in.z  = vec_in.z  - 100 ;  // 
float3 Draw_multiple_obj::Vec_rotation ( float3 vec_in , GLfloat * Rotation_4_by_4 )
{
	vec_in.z  = vec_in.z  - 100 ; // 
	float3 vec_out ; 
	vec_out.x = Rotation_4_by_4[0+0] *  vec_in.x  + Rotation_4_by_4[1+0] *  vec_in.y + Rotation_4_by_4[2+0] *  vec_in.z   ; 
	vec_out.y = Rotation_4_by_4[0+4] *  vec_in.x  + Rotation_4_by_4[1+4] *  vec_in.y + Rotation_4_by_4[2+4] *  vec_in.z   ; 
	vec_out.z = Rotation_4_by_4[0+8] *  vec_in.x  + Rotation_4_by_4[1+8] *  vec_in.y + Rotation_4_by_4[2+8] *  vec_in.z   ; //  
	return vec_out ; // 
}
// 
float3 Draw_multiple_obj::Vec_rotation_2 ( float3 vec_in , GLfloat * Rotation_4_by_4 )
{
	float3 vec_out ; 
	vec_out.x = Rotation_4_by_4[0+0] *  vec_in.x  + Rotation_4_by_4[1+0] *  vec_in.y + Rotation_4_by_4[2+0] *  vec_in.z   ; 
	vec_out.y = Rotation_4_by_4[0+4] *  vec_in.x  + Rotation_4_by_4[1+4] *  vec_in.y + Rotation_4_by_4[2+4] *  vec_in.z   ; 
	vec_out.z = Rotation_4_by_4[0+8] *  vec_in.x  + Rotation_4_by_4[1+8] *  vec_in.y + Rotation_4_by_4[2+8] *  vec_in.z   ; //  
	return vec_out ; // 
}

//  ******************* 普通渲染 *******************  // 
void Draw_multiple_obj::render() // 
{  //   
	m_baiscobj->Process_Leap_Motion() ;   //    调用Leap_Motion  // 
	this->Projection_Setup()          ;   //  包含了旋转矩阵拷贝 //  WW //  

	glsl_obj_softbody->model_view[0] = 1;
	glsl_obj_softbody->model_view[1] = 0;
	glsl_obj_softbody->model_view[2] = 0;
	glsl_obj_softbody->model_view[3] = 0;
	//
	glsl_obj_softbody->model_view[4] = 0;
	glsl_obj_softbody->model_view[5] = 1;
	glsl_obj_softbody->model_view[6] = 0;
	glsl_obj_softbody->model_view[7] = 0;
	//
	glsl_obj_softbody->model_view[8] = 0;
	glsl_obj_softbody->model_view[9] = 0;
	glsl_obj_softbody->model_view[10] = 1;
	glsl_obj_softbody->model_view[11] = 0;
	//
	glsl_obj_softbody->model_view[12] = 0;
	glsl_obj_softbody->model_view[13] = 0;
	glsl_obj_softbody->model_view[14] = 100;
	glsl_obj_softbody->model_view[15] = 1;
	
	if (  Flag__GRASP  > 0.5 )             //  
	{   // 
		glsl_obj_ball_marker->model_view[12] = glsl_obj_ball_marker->model_view[12] - glsl_obj_ball_marker->model_view[12 - 4] * 80; //
		glsl_obj_ball_marker->model_view[13] = glsl_obj_ball_marker->model_view[13] - glsl_obj_ball_marker->model_view[13 - 4] * 80; //
		glsl_obj_ball_marker->model_view[14] = glsl_obj_ball_marker->model_view[14] - glsl_obj_ball_marker->model_view[14 - 4] * 80 + 100 ; //  
		// 
		glsl_obj_ball_marker->GLSL_Vertex_Frag_Enable2()  ; // 
		glsl_obj_ball_marker->GLSL_Vertex_Frag_Disable2() ; // 
		//  
		glsl_obj_poster->model_view[12] = glsl_obj_ball_marker->model_view[12] + 40; //
		glsl_obj_poster->model_view[13] = glsl_obj_ball_marker->model_view[13] + 20; //
		glsl_obj_poster->model_view[14] = 400; //  
		// 
		float3 Probe_stylus_after_rotation = Vec_rotation(float3(glsl_obj_ball_marker->model_view[12], glsl_obj_ball_marker->model_view[13], glsl_obj_ball_marker->model_view[14]), glsl_obj_softbody->model_view); // 
		Organ_num = Organ_Indentification ( Grasped_Vertex_ID );
		// 
		if ( Organ_num >= 0)  // 
		{  
			for (int i = 0; i < glsl_obj_poster->Mesh_Size; i++) // 
			{  
				glsl_obj_poster->pvertex[i].s0 = glsl_obj_poster->pvertex_backup[i].s0 + Denotion_texture_remapping[Organ_num][0] * 0.5   ; // 
				glsl_obj_poster->pvertex[i].t0 = glsl_obj_poster->pvertex_backup[i].t0 + Denotion_texture_remapping[Organ_num][1] * 0.125 ; // 
			}
			// *********************** test *********************** // 
			glBindBuffer(GL_ARRAY_BUFFER, glsl_obj_poster->gVertexBuffer);  // 绑定buffer  GL_DYNAMIC_DRAW  GL_STATIC_DRAW
			glBufferData(GL_ARRAY_BUFFER, sizeof(MyVertex) *glsl_obj_poster->Mesh_Size, &glsl_obj_poster->pvertex[0].x, GL_DYNAMIC_DRAW); // 
			glIndexPointer(GL_DOUBLE, sizeof(int) * 3, BUFFER_OFFSET(0));   //The starting point of texcoords, 24 bytes away
																			// 
			glVertexPointer(3, GL_FLOAT, sizeof(MyVertex)   , BUFFER_OFFSET(0));   //The starting point of the VBO, for the vertices
			glNormalPointer(GL_FLOAT, sizeof(MyVertex)      , BUFFER_OFFSET(sizeof(float) * 3 ) );   //The starting point of the VBO, for the vertices
			glTexCoordPointer(2, GL_FLOAT, sizeof(MyVertex) , BUFFER_OFFSET(sizeof(float) * 6 ) );   //The starting point of texcoords, 24 bytes away 
			 
			glDisable(GL_CULL_FACE);
			glsl_obj_poster->GLSL_Vertex_Frag_Enable2();   // 
			glsl_obj_poster->GLSL_Vertex_Frag_Disable2();
			glEnable(GL_CULL_FACE); 
			// std::cout << "Organ_num = " << Organ_num << endl ;  //  
		}
	} 	//  //


	NxMat34 cutter_pose   ; //   //  
	NxMat33 cutter_orient ; //   //   
	NxVec3  velocity_cgw  = NxVec3( m_baiscobj->listener->speed_cgw.x, m_baiscobj->listener->speed_cgw.y, m_baiscobj->listener->speed_cgw.z);
	float   glmat[16]     ; //   // 
	memcpy ( glmat , glsl_obj_cutter->model_view , 16 * sizeof(float) ) ; //   
	glmat[13]  =  glmat[13] ; //   
	cutter_pose.setColumnMajor44( glmat ); //  
	cutter_orient.setColumnMajorStride4(glmat); //   


												  //glsl_obj_softbody->RenderCall_Back(  cutter_pose   ) ; //       
												  //glsl_obj_softbody->Normal_Update()    ;  //    

												  // glsl_obj_softbody->RenderCall_Back( velocity_cgw , cutter_orient);//
												  // glsl_obj_softbody->Normal_Update();  //    

	if (m_baiscobj->listener->HandNum >  0.5)
	{
		velocity_cgw = NxVec3(m_baiscobj->listener->speed_cgw.x , m_baiscobj->listener->speed_cgw.y , m_baiscobj->listener->speed_cgw.z );
		// velocity_cgw = NxVec3(1,1,1);
	}
	else
	{
		velocity_cgw = NxVec3(0,0,0); 
	}  
	// 
	// 

	clock_t start = clock();  
	glsl_obj_softbody->RenderCall_Back( velocity_cgw/1.0 , cutter_orient);
	glsl_obj_softbody->Normal_Update();  //     
	clock_t end = clock();
	float seconds = (float)(end - start) / 1000;
	cout << "PhysX simulate = " << seconds << endl;



	cutter_pose = glsl_obj_softbody->actor->getGlobalPose();
	cutter_pose.getColumnMajor44(glsl_obj_cutter->model_view); 
    
	glsl_obj_cutter->model_view[14] = glsl_obj_cutter->model_view[14] + 100;  //   
	glsl_obj_cutter->GLSL_Vertex_Frag_Enable2();   // 
	glsl_obj_cutter->GLSL_Vertex_Frag_Disable2();   //    
	glsl_obj_cutter->model_view[14] = glsl_obj_cutter->model_view[14] - 100;  //   


	//float *tmp = glsl_obj_cutter->model_view    ;  //  
	//glmat[14] = glmat[14] + 100  ;  //   
	//// 
	//glsl_obj_cutter->model_view = glmat ;
	//glsl_obj_cutter->GLSL_Vertex_Frag_Enable2()  ;   // 
	//glsl_obj_cutter->GLSL_Vertex_Frag_Disable2() ;   //   
	//glsl_obj_cutter->model_view = tmp            ;   // 		 
	//// 
	//// 



	// 
	glBindBuffer(GL_ARRAY_BUFFER, glsl_obj_softbody->gVertexBuffer);  // 绑定buffer  GL_DYNAMIC_DRAW  GL_STATIC_DRAW
	glBufferData(GL_ARRAY_BUFFER, sizeof(MyVertex)*glsl_obj_softbody->Mesh_Size, &glsl_obj_softbody->pvertex[0].x, GL_DYNAMIC_DRAW); //      
	// glVertexPointer(3, GL_FLOAT, sizeof(float) * 0, glsl_obj_softbody->received_vertexes);    //  
	glVertexPointer(3, GL_FLOAT, sizeof(MyVertex), BUFFER_OFFSET(sizeof(float) * 0));    // 
	glNormalPointer(GL_FLOAT, sizeof(MyVertex), BUFFER_OFFSET(sizeof(float) * 3)); // The starting point of the VBO, for the vertices
	glTexCoordPointer(2, GL_FLOAT, sizeof(MyVertex), BUFFER_OFFSET(sizeof(float) * 6)); //  

	glsl_obj_softbody->GLSL_Vertex_Frag_Enable2( 1 );   //  
	glsl_obj_softbody->GLSL_Vertex_Frag_Disable2();   //    
													  //
}
// 
void Draw_multiple_obj::render_new() {}  //  
void Draw_multiple_obj::render_multiple_iv_back()  { }    
void Draw_multiple_obj::render_back_surface() {}  
void Draw_multiple_obj::render_Front_surface() {} 
//   
void Draw_multiple_obj::render_multiple_iv_front()
{
	m_baiscobj->Process_Leap_Motion();   //  Meffy: 调用Leap_Motion  // cgw comment this //   
	this->Projection_Setup();            //  包含了旋转矩阵拷贝 //  WW //  
	//                       // 
	// glsl_obj_softbody-> ; //    glsl_obj_softbody-> ; // 
	// cout << glsl_obj_softbody->model_view[14] << endl;  
	glsl_obj_softbody->model_view[0]  = 1;
	glsl_obj_softbody->model_view[1]  = 0;
	glsl_obj_softbody->model_view[2]  = 0;
	glsl_obj_softbody->model_view[3]  = 0;

	glsl_obj_softbody->model_view[4]  = 0;
	glsl_obj_softbody->model_view[5]  = 1;
	glsl_obj_softbody->model_view[6]  = 0;
	glsl_obj_softbody->model_view[7]  = 0;

	glsl_obj_softbody->model_view[8] = 0;
	glsl_obj_softbody->model_view[9] = 0;
	glsl_obj_softbody->model_view[10] = 1; 
	glsl_obj_softbody->model_view[11] = 0;
	// 
	glsl_obj_softbody->model_view[12] = 0;
	glsl_obj_softbody->model_view[13] = 0;
	glsl_obj_softbody->model_view[14] = 100;
	glsl_obj_softbody->model_view[15] = 1; 
	// 
	NxMat34 cutter_pose; //   //   
	float glmat[16];  // 
	memcpy(glmat, glsl_obj_cutter->model_view, 16 * sizeof(float)); //  
	glmat[13] = glmat[13]; //  
	cutter_pose.setColumnMajor44(glmat); //  


	glClearDepth(0); // 深度清理成最深0 开始绘制正面 
	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);  // 开启深度测试;
	glDepthFunc(GL_GREATER);  // 开启较远显示;
	glEnable(GL_CULL_FACE);  // 开启面裁剪
	glCullFace(GL_FRONT);  // 将较近的面裁剪掉  
	 


	glsl_obj_softbody->RenderCall_Back(cutter_pose);//     
	glsl_obj_softbody->Normal_Update();  //    

	mult_camera->Bound_box_Init[0] = glsl_obj_softbody->mesh.bounding_box[0]; 
	mult_camera->Bound_box_Init[1] = glsl_obj_softbody->mesh.bounding_box[1];
	mult_camera->Bound_box_Init[2] = glsl_obj_softbody->mesh.bounding_box[2];
	mult_camera->View_Render_Flag_Update();  

	glBindBuffer(GL_ARRAY_BUFFER, glsl_obj_softbody->gVertexBuffer);  // 绑定buffer  GL_DYNAMIC_DRAW  GL_STATIC_DRAW
	glBufferData(GL_ARRAY_BUFFER, sizeof(MyVertex)*glsl_obj_softbody->Mesh_Size, &glsl_obj_softbody->pvertex[0].x, GL_DYNAMIC_DRAW); //      
																																	 // glVertexPointer(3, GL_FLOAT, sizeof(float) * 0, glsl_obj_softbody->received_vertexes);    //  
	glVertexPointer(3, GL_FLOAT, sizeof(MyVertex), BUFFER_OFFSET(sizeof(float) * 0));    // 
	glNormalPointer(GL_FLOAT, sizeof(MyVertex), BUFFER_OFFSET(sizeof(float) * 3)); // The starting point of the VBO, for the vertices
	glTexCoordPointer(2, GL_FLOAT, sizeof(MyVertex), BUFFER_OFFSET(sizeof(float) * 6)); //  
																						// 
																						// 
	for (int i = 0; i < mult_camera->LENS_NUM_X; i++)
	{
		for (int j = 0; j < mult_camera->LENS_NUM_Y; j++)
		{
			if (mult_camera->View_Render_Flag[i][j][0])
			{
				glsl_obj_softbody->project_matrix = mult_camera->Multiple_Lens_Project_Matrix_Front[j + i * mult_camera->LENS_NUM_Y]; // 
																																	  // 
				glViewport(mult_camera->Multiple_Viewport[i][j][0], mult_camera->Multiple_Viewport[i][j][1], mult_camera->Multiple_Viewport[i][j][2], mult_camera->Multiple_Viewport[i][j][3]);

				glsl_obj_softbody->GLSL_Vertex_Frag_Enable2(1);   //  
				glsl_obj_softbody->GLSL_Vertex_Frag_Disable2();   //     
			}
		}
	}
	//    // 
	//= glsl_obj_cutter->project_matrix = glsl_obj_ball_marker->project_matrix =
	//	glsl_obj_poster->project_matrix 



	for (int i = 0; i < mult_camera->LENS_NUM_X; i++)
	{
		for (int j = 0; j < mult_camera->LENS_NUM_Y; j++)
		{
			if (mult_camera->View_Render_Flag[i][j][0])
			{
				glsl_obj_cutter->project_matrix = mult_camera->Multiple_Lens_Project_Matrix_Front[j + i * mult_camera->LENS_NUM_Y]; //  
				glViewport(mult_camera->Multiple_Viewport[i][j][0], mult_camera->Multiple_Viewport[i][j][1], mult_camera->Multiple_Viewport[i][j][2], mult_camera->Multiple_Viewport[i][j][3]);

				glsl_obj_cutter->GLSL_Vertex_Frag_Enable2();   //  
				glsl_obj_cutter->GLSL_Vertex_Frag_Disable2();   //     
			}
		}
	}
	//    // 
	if (Flag__GRASP > 0.5) // 
	{   // 
		glsl_obj_ball_marker->model_view[12] = glsl_obj_ball_marker->model_view[12] - glsl_obj_ball_marker->model_view[12 - 4] * 80; //
		glsl_obj_ball_marker->model_view[13] = glsl_obj_ball_marker->model_view[13] - glsl_obj_ball_marker->model_view[13 - 4] * 80; //
		glsl_obj_ball_marker->model_view[14] = glsl_obj_ball_marker->model_view[14] - glsl_obj_ball_marker->model_view[14 - 4] * 80 + 100; //  
																																		   // 
		glsl_obj_ball_marker->GLSL_Vertex_Frag_Enable2(); // 
		glsl_obj_ball_marker->GLSL_Vertex_Frag_Disable2(); //  
														   ///  
		glsl_obj_poster->model_view[12] = glsl_obj_ball_marker->model_view[12] + 40; //
		glsl_obj_poster->model_view[13] = glsl_obj_ball_marker->model_view[13] + 20; //
		glsl_obj_poster->model_view[14] = 170; //  
											   // 
		float3 Probe_stylus_after_rotation = Vec_rotation(float3(glsl_obj_ball_marker->model_view[12], glsl_obj_ball_marker->model_view[13], glsl_obj_ball_marker->model_view[14]), glsl_obj_softbody->model_view); // 
		Organ_num = Organ_Indentification(Grasped_Vertex_ID);
		// 
		if (Organ_num >= 0)  // 
		{
			for (int i = 0; i < glsl_obj_poster->Mesh_Size; i++) // 
			{
				glsl_obj_poster->pvertex[i].s0 = glsl_obj_poster->pvertex_backup[i].s0 + Denotion_texture_remapping[Organ_num][0] * 0.5; // 
				glsl_obj_poster->pvertex[i].t0 = glsl_obj_poster->pvertex_backup[i].t0 + Denotion_texture_remapping[Organ_num][1] * 0.125; // 
			}
			// *********************** test *********************** // 
			glBindBuffer(GL_ARRAY_BUFFER, glsl_obj_poster->gVertexBuffer);  // 绑定buffer  GL_DYNAMIC_DRAW  GL_STATIC_DRAW
			glBufferData(GL_ARRAY_BUFFER, sizeof(MyVertex) *glsl_obj_poster->Mesh_Size, &glsl_obj_poster->pvertex[0].x, GL_DYNAMIC_DRAW); // 
			glIndexPointer(GL_DOUBLE, sizeof(int) * 3, BUFFER_OFFSET(0));   //The starting point of texcoords, 24 bytes away
																			// 
			glVertexPointer(3, GL_FLOAT, sizeof(MyVertex), BUFFER_OFFSET(0));   //The starting point of the VBO, for the vertices
			glNormalPointer(GL_FLOAT, sizeof(MyVertex), BUFFER_OFFSET(sizeof(float) * 3));   //The starting point of the VBO, for the vertices
			glTexCoordPointer(2, GL_FLOAT, sizeof(MyVertex), BUFFER_OFFSET(sizeof(float) * 6));   //The starting point of texcoords, 24 bytes away 

			glDisable(GL_CULL_FACE);
			glsl_obj_poster->GLSL_Vertex_Frag_Enable2();   // 
			glsl_obj_poster->GLSL_Vertex_Frag_Disable2();
			glEnable(GL_CULL_FACE);
			// std::cout << "Organ_num = " << Organ_num << endl ;  //  
		}
	} 	//  //
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
}

/* int Flag___    =  0   ; //
	int Organ_num  = -1   ; //
	m_baiscobj->Process_Leap_Motion() ;   //  Meffy: 调用Leap_Motion  // cgw comment this //
	this->Projection_Setup() ;            //  包含了旋转矩阵拷贝 //  WW //

	//
	if( m_baiscobj->listener->HandNum > 0 )
	{
		glsl_obj_softbody->Probe_Hand_Origin    =    float3 ( m_baiscobj->Transform_Matrix_Leap[12] - glsl_obj_ball_marker->model_view[8]   ,
															  m_baiscobj->Transform_Matrix_Leap[13] - glsl_obj_ball_marker->model_view[9]   ,
															  m_baiscobj->Transform_Matrix_Leap[14] - glsl_obj_ball_marker->model_view[10]  )  ;  //

		glsl_obj_softbody->Probe_Hand_direction =   float3 ( - m_baiscobj->Transform_Matrix_Leap[8]   , - m_baiscobj->Transform_Matrix_Leap[9]   ,    -m_baiscobj->Transform_Matrix_Leap[10]  )  ; //
		//
		if ( glsl_obj_softbody->Intersect_leap_probe(  m_baiscobj->Transform_Matrix_Mouse  )  )    // system( "pause" ) ; //
		{ 	memcpy (  glsl_obj_ball_marker->model_view  , m_baiscobj->Transform_Matrix_Leap   ,  16*sizeof( GLfloat )   )   ;    //
			glsl_obj_ball_marker->model_view[12] = m_baiscobj->Transform_Matrix_Leap[12]  - glsl_obj_ball_marker->model_view[8]  * 80 ; //
			glsl_obj_ball_marker->model_view[13] = m_baiscobj->Transform_Matrix_Leap[13]  - glsl_obj_ball_marker->model_view[9]  * 80 ; //
			glsl_obj_ball_marker->model_view[14] = m_baiscobj->Transform_Matrix_Leap[14]  - glsl_obj_ball_marker->model_view[10] * 80 ; //   
			// 
			glsl_obj_poster->model_view[12] = glsl_obj_ball_marker->model_view[12]   + 0    ; //
			glsl_obj_poster->model_view[13] = glsl_obj_ball_marker->model_view[13]   - 30   ; //
			glsl_obj_poster->model_view[14] = max( float(140.0) , glsl_obj_ball_marker->model_view[14]  + 10 )   ; //   

			float3 Probe_stylus_after_rotation    =  Vec_rotation   ( float3( glsl_obj_ball_marker->model_view[12] , glsl_obj_ball_marker->model_view[13],  glsl_obj_ball_marker->model_view[14]  )    ,  m_baiscobj->Transform_Matrix_Mouse ) ; // 
			this->Organ_num = Organ_Indentification( Grasped_Vertex_ID ) ; //  
			 // 
			if( this->Organ_num >= 0  ) 
			{
				glsl_obj_softbody->temporary_count_ =  0 ;  Flag___ = 1 ;    
				// 
				for (int i = 0 ; i < glsl_obj_poster->Mesh_Size ; i++  )
				{ 
					glsl_obj_poster->pvertex[i].s0 =  glsl_obj_poster->pvertex_backup[i].s0 + Denotion_texture_remapping[Organ_num][0] * 0.5 ; 
					glsl_obj_poster->pvertex[i].t0 =  glsl_obj_poster->pvertex_backup[i].t0 + Denotion_texture_remapping[Organ_num][1] * 0.125 ; 
				}
				// *********************** test *********************** // 
				glBindBuffer   (GL_ARRAY_BUFFER, glsl_obj_poster->gVertexBuffer);  // 绑定buffer  GL_DYNAMIC_DRAW  GL_STATIC_DRAW
				glBufferData   (GL_ARRAY_BUFFER, sizeof(MyVertex) *glsl_obj_poster->Mesh_Size, &glsl_obj_poster->pvertex[0].x, GL_DYNAMIC_DRAW ); // 
				glIndexPointer (GL_DOUBLE,  sizeof(int)*3  , BUFFER_OFFSET(0) );   //The starting point of texcoords, 24 bytes away
				// 
				glVertexPointer  (  3         , GL_FLOAT, sizeof(MyVertex)  , BUFFER_OFFSET(0));   //The starting point of the VBO, for the vertices
				glNormalPointer  (  GL_FLOAT   ,  sizeof(MyVertex)     , BUFFER_OFFSET(sizeof(float)*3 ));   //The starting point of the VBO, for the vertices
				glTexCoordPointer(  2      , GL_FLOAT, sizeof(MyVertex), BUFFER_OFFSET(sizeof(float)*6 ));   //The starting point of texcoords, 24 bytes away 
			}
		}  //  
	} //  
	memcpy ( glsl_obj_softbody->model_view     , m_baiscobj->Transform_Matrix_Mouse   ,  16*sizeof( GLfloat )   )   ;    //  glsl_obj_softbody->model_view[14]  = glsl_obj_softbody->model_view[14]  + 100     ;  
	memcpy ( glsl_obj_cutter->model_view       , m_baiscobj->Transform_Matrix_Leap   ,  16*sizeof( GLfloat )   )   ;    // 
 	// 
	mult_camera->Bound_box_Init[0]  = glsl_obj_softbody->mesh.bounding_box[0] ; //  
	mult_camera->Bound_box_Init[1]  = glsl_obj_softbody->mesh.bounding_box[1] ; //   
	// 
	memcpy( &mult_camera->Identity_Matrix[0][0]  , m_baiscobj->Transform_Matrix_Mouse , 16*sizeof( GLfloat ) ); // 
 	glClearDepth   (     0     ) ; // 深度清理成最深0 开始绘制正面 
	glClear(GL_DEPTH_BUFFER_BIT) ;
	glEnable   ( GL_DEPTH_TEST) ;  // 开启深度测试;
	glDepthFunc( GL_GREATER   ) ;  // 开启较远显示;
	glEnable   ( GL_CULL_FACE ) ;  // 开启面裁剪
	glCullFace ( GL_FRONT     ) ;  // 将较近的面裁剪掉    



	int length_tmp , index_retangle  ;  //  
	for ( int i = 0 ; i < mult_camera->LENS_NUM_X ; i++ )
	{ 
		for ( int j = 0 ; j < mult_camera->LENS_NUM_Y ; j++ )
		{    //  Front  part  // 
			index_retangle = i * mult_camera->LENS_NUM_Y + j ; // 
			if( 1 ) // 
			{  
				glsl_obj_softbody->project_matrix     = 
				glsl_obj_cutter->project_matrix       =
				glsl_obj_ball_marker->project_matrix  = 
				glsl_obj_poster->project_matrix       = 
				mult_camera->Multiple_Lens_Project_Matrix_Front [ j + i * mult_camera->LENS_NUM_Y ] ; // 
				// 
				glViewport( mult_camera->Multiple_Viewport[i][j][0] , mult_camera->Multiple_Viewport[i][j][1] ,mult_camera->Multiple_Viewport[i][j][2] ,mult_camera->Multiple_Viewport[i][j][3] );
				//  
				// length_tmp  = Face_Limit_Rectangle[ index_retangle][1] - Face_Limit_Rectangle[index_retangle][0] ;  
				//
				//glsl_obj_softbody->GLSL_Vertex_Frag_Enable3 ( Face_Limit_Rectangle[index_retangle][0]  , length_tmp  ) ;  
				//glsl_obj_softbody->GLSL_Vertex_Frag_Disable3 ( ) ;     
				// glsl_obj_softbody->GLSL_Vertex_Frag_Enable2(1);   //  
				// glsl_obj_softbody->GLSL_Vertex_Frag_Disable2();   //   

				// // 
				if( m_baiscobj->listener->HandNum > 0 )
				{
					glsl_obj_cutter->GLSL_Vertex_Frag_Enable2 () ;  // 
					glsl_obj_cutter->GLSL_Vertex_Frag_Disable2() ;  //  
				}
				if( this->Flag__GRASP > 0.5 )
				{ 
					glsl_obj_ball_marker->GLSL_Vertex_Frag_Enable2() ;   // 
					glsl_obj_ball_marker->GLSL_Vertex_Frag_Disable3 ( ) ;   
					glsl_obj_poster->GLSL_Vertex_Frag_Enable2()     ;   // 
					glsl_obj_poster->GLSL_Vertex_Frag_Disable3 ( ) ;   
				}

			} 
		}
	}
 	glDisable(GL_DEPTH_TEST) ;
	glDisable(GL_CULL_FACE); 
	*/
// 
// 
void Draw_multiple_obj::renderSingle_Lens()  // 采用裁剪切平面的方法 
{ } 

void Draw_multiple_obj::render_glsl()
{ }


#pragma region Save_Space 
//

// 
void Draw_multiple_obj::myDisplay()
{ 
	
	 clock_t start = clock();   
	 iteration_of_display++; 
	 // Triangle_Primitive_Determine( glsl_obj_softbody->mesh.obj_Octree ) ;  // 
	 if (iteration_of_display < -1 )
	 {
		 screen_obj->If_SaveImage = 1 ;  // 
		 sprintf(screen_obj->filename_BMP, "Images\\%d.bmp", iteration_of_display);
	 }



 	if(  screen_obj->Msaa_Flag  ) 
	{
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT ,  screen_obj->MSAA_fbo ) ;  // windows 默认的分辨率 0   //Meffy：绑定fbo与帧缓冲区
	}
	else
	{
		if(screen_obj->If_SaveImage)
		{
			// m_baiscobj->Matrix_Vector_Reset(    ) ; // 
			glBindFramebufferEXT(  GL_FRAMEBUFFER_EXT  ,  screen_obj->window_fbo );  // windows 默认的分辨率 0 
			
		}
		else 
		{
			glBindFramebufferEXT(  GL_FRAMEBUFFER_EXT  ,  0  );  // windows 默认的分辨率 0 
		}
	}

	glViewport( 0 , 0 , mult_camera->WIDTH , mult_camera->HEIGHT ) ;    //Meffy:设置视口大小
	glClear    (  GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT  ) ;  
	glClearColor(0.0 , 0.0 , 0 , 0.0 );
	fps_obj->frame_calculate();
	fps_obj->frame_display  () ;

#pragma region ss

#pragma endregion justbefore 
	// start   = clock();
	switch (menu_display)
	{
	case '0': 
		// render(); 
		render();
		break;
	case '1':
		// glsl_obj_softbody->GLSL_INIT();   
		renderSingle_Lens();
		break;
	case '2':
		render_back_surface();
		break;
	case '3':
		render_Front_surface();
		break;
	case '4': 
		//clock_t start   = clock();  
		render_multiple_iv_back();
		//clock_t end     = clock(); 
		//float seconds   = (float)(end - start) / 1000 ;
		//cout<< "Render_multiple_iv_back iteration time cost = " <<seconds<<endl ;  
		break; // 
	case '5':
		// glsl_obj_softbody->GLSL_INIT_2();   
		render_multiple_iv_front();
		break;
	case '6':
		render_new();
		break;
	default: 
		render (); 
		// render_multiple_iv_front(); 
		break;
	}

	// end       = clock(); 
	// seconds   =  (end - start) / 1000.0 ;
	// cout<< "Render_multiple_iv_back iteration time cost = " <<seconds<<endl ;  

	if( screen_obj->Msaa_Flag )
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0 );   // Make sure no FBO is set as the draw framebuffer
		glBindFramebuffer(GL_READ_FRAMEBUFFER, screen_obj->MSAA_fbo); // Make sure your multisampled FBO is the read framebuffer
		glDrawBuffer(GL_BACK);                       // Set the back buffer as the draw buffer
		glBlitFramebuffer(0, 0,mult_camera->WIDTH , mult_camera->HEIGHT , 0, 0, mult_camera->WIDTH , mult_camera->HEIGHT , GL_COLOR_BUFFER_BIT, GL_NEAREST ); // GL_NEAREST GL_LINEAR
	}
	else
	{
		if ( screen_obj->If_SaveImage   )    // 技术性的解决了这个问题 // 
		{ 
			screen_obj->GET_Texture(  screen_obj->window_image , mult_camera->WIDTH , mult_camera->HEIGHT ) ;
			screen_obj->Disable_Save(    ) ;

			glBindFramebufferEXT(  GL_FRAMEBUFFER_EXT ,  0  );  // windows 默认的分辨率 0 
			glViewport(  0  , 0 , mult_camera->WIDTH  , mult_camera->HEIGHT  ) ;   // Meffy:设置视口大小
			glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)  ; //  glClear(GL_DEPTH_BUFFER_BIT); 
			glClearColor(1, 1, 0, 0.5 );
			glClearDepth(1);
			glEnable(GL_DEPTH_TEST) ; 
			glDepthFunc( GL_LESS ) ;
			 
			glActiveTexture ( GL_TEXTURE0 ) ; glEnable(GL_TEXTURE_2D   ); 
			glBindTexture   ( GL_TEXTURE_2D , screen_obj->window_image ); // 选择纹理    

			glBegin(GL_QUADS);									// Draw A Quad
			glTexCoord2d(0.0,0.0); glVertex2d(-1.0,-1.0);
			glTexCoord2d(1.0,0.0); glVertex2d( 1.0,-1.0);
			glTexCoord2d(1.0,1.0); glVertex2d( 1.0,1.0);
			glTexCoord2d(0.0,1.0); glVertex2d(-1.0,1.0);
			glEnd();											        // Done Drawing The Quad 

			cout << "Glending " << endl; 

		}
	}
	glDisable(GL_TEXTURE_2D) ;  
	glDisable(GL_DEPTH_TEST) ; 
	glutSwapBuffers();    



	clock_t end = clock();

	float seconds = (float)(end - start) / 1000;
	cout << "Triangle_Primitive_Determine overhead = " << seconds << endl;
}
//

void Draw_multiple_obj::Load_csv( )  
{	ifstream inFile("liver_denotion.csv", ios::in);  
    string lineStr;  // 

    while (getline(inFile, lineStr))  
    {   
        stringstream ss(lineStr);  
        string str;  
        vector<string> lineArray;  
        // 按照逗号分隔  
        while (getline(ss, str, ','))  
            lineArray.push_back(str);  
        strArray.push_back(lineArray);    
    }  

	// 
	int count = strArray.size()  ;  // 
	Organ  = (double ** ) malloc ( count * sizeof(double) ) ; // 
	for ( int i = 0 ; i<count ; i++ )  // 
	{ 
		Organ[i]=(double *)malloc(strArray[0].size()*sizeof(double) ) ; // 
	} 
	// 
    for (int i = 0; i < count;i++)
    {
		for (int j = 0; j < strArray[i].size(); j++)   // 
		{
			std::cout << strArray[i][j] << endl;
			Organ[i][j] = atof(strArray[i][j].c_str()); //   
		}
	}
} 
//
void Draw_multiple_obj::Load_Pipeline2( )
{ // 不对 需要修改
	glsl_obj_softbody->GLSL_Vertex_Frag_Enable2() ;
}

void Draw_multiple_obj::Disable_PipeLine2()
{
	glsl_obj_softbody->GLSL_Vertex_Frag_Disable2() ;
}

//
void Draw_multiple_obj::Load_Pipeline(float center_x , float center_y , float window_x , float window_y, float pitch, float pitchy ,float backfront_flag , GLdouble *clip_plane  )
{ // 不对 需要修改
	glsl_obj_softbody->GLSL_Vertex_Frag_Enable(center_x, center_y , window_x, window_y , pitch , pitchy, backfront_flag , clip_plane) ;
}

void Draw_multiple_obj::Disable_PipeLine()
{
	glsl_obj_softbody->GLSL_Vertex_Frag_Disable() ;
}

void Draw_multiple_obj::Keyboard(unsigned char key, int x, int y)
{
	// detect current settings
	GLint iMultiSample = 20;
	GLint iNumSamples  = 20; 
	switch (key) {  

	case 'm':  
		glutSetOption(GLUT_MULTISAMPLE, 16 );
		glGetIntegerv(GL_SAMPLE_BUFFERS, &iMultiSample);
		glGetIntegerv(GL_SAMPLES, &iNumSamples);
		printf("MSAA on, GL_SAMPLE_BUFFERS = %d, GL_SAMPLES = %d\n", iMultiSample, iNumSamples);  
		break;
	case 's':  
		Window_size = Window_size * 0.98 ;
		Reshape(window_w,window_h) ;
		glutPostRedisplay(); 
		break;
	case 'b':  
		screen_obj->Process_Key() ; 
		break; 
	case 'v':  
		glutPostRedisplay(); 
		break ; 
	case'f':
		if (~Full_Screen_Flag)
		{
			Full_Screen_Flag   = true ; 
			glutFullScreen();
			glutPostRedisplay(); 
		}
		break;  
	case 27:  
		if (Full_Screen_Flag)
		{
			Full_Screen_Flag   = false; 
			glutReshapeWindow  ( int (window_w/2) , int (window_h/2) ); 
			glutPositionWindow ( 100,110 );
		}
		else
		{
			// screen_obj->MSAA_fbo  ; 
			glDeleteBuffers(1, &screen_obj->window_fbo  ) ;
			glDeleteBuffers(1, &screen_obj->window_image) ;
			glDeleteBuffers(1, &screen_obj->window_oid  ) ; 
			// 
			glDeleteBuffers(1, &screen_obj->MSAA_fbo  ) ;
			glDeleteBuffers(1, &screen_obj->m_renderBufferColorMS  ) ; 
			glDeleteBuffers(1, &screen_obj->m_renderBufferDepthMS  ) ; 
			// 
			mult_camera->Multiple_Viewport_Free()            ; // 
			mult_camera->Multiple_Lens_Project_Matrix_Free() ; // 
			mult_camera->View_Render_Flag_Free()             ; // 
			// 
			m_baiscobj->listener->onDisconnect ( * m_baiscobj->listener ) ; // 退出系统
			m_baiscobj->listener->onExit ( * m_baiscobj->listener ) ;       // 退出系统
			fclose( fps_obj->fp111 ); 
			// 
			for(int i = 0; i < mult_camera->MLA_NUMBDER_RECTANGLE ; i ++) // 
			{	
				free( Face_Limit_Rectangle[i] ) ;  
			} 
			free( Face_Limit_Rectangle ) ;  Face_Limit_Rectangle= NULL ; // 
			// 
			for(int i = 0; i < mult_camera->MLA_NUMBDER_HEXAGON ; i ++) // 
			{	
				free( Face_Limit_Hexagon[i] ) ;  
			} 
			free( Face_Limit_Hexagon ) ;  Face_Limit_Hexagon= NULL ; // 
			// 
			exit(0);   // 
		}
		break;  
	default:
		m_baiscobj->keyboard(  key,  x,  y)   ;
		break;
	}  
}
// 
void Draw_multiple_obj::Idle() 
{
	m_baiscobj->Idle() ;
}
// 

Draw_multiple_obj::~Draw_multiple_obj() {}

void Draw_multiple_obj::InitGL(GLvoid)    // Create Some Everyday Functions  
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);  //GLUT_SINGLE  | GLUT_MULTISAMPLE
	glutInitWindowPosition(0, 0);  glutInitWindowSize(mult_camera->WIDTH, mult_camera->HEIGHT);   //设置窗口大小 
	std::cout << mult_camera->WIDTH << endl; std::cout << mult_camera->HEIGHT << endl;  
	glutCreateWindow(" OpenGL Liver Softsimulation ");

	glEnable(GL_DEPTH_TEST); glDepthMask(GL_TRUE); 	glDepthFunc(GL_LESS);
	// 前面运行了glut*的一系列函数，已经获得了opengl的context，所以这里不会出错，读者可以试试在main的开始就调用这句会怎样  
	GLenum err = glewInit(); if (err != GLEW_OK) {  fprintf(stderr, "%s\n", glewGetErrorString(err)); }   

	glsl_obj_cutter->Model_Load(); // 
	glsl_obj_cutter->LoadGLTextures("Data\\stylus_blue.bmp");
	glsl_obj_cutter->GLSL_INIT_2(); //  
	// 
	glsl_obj_softbody->GLSL_INIT_2( ) ;
	glsl_obj_softbody->Model_Load_Soft_Simulation ( glsl_obj_cutter->mesh.bounding_box[1] - glsl_obj_cutter->mesh.bounding_box[0]) ; //   
	glsl_obj_softbody->LoadGLTextures("Data\\Liver_resized_to_2_Physx_export_Tet.bmp");
	//  
	glsl_obj_ball_marker->Model_Load(); // 
	glsl_obj_ball_marker->LoadGLTextures("Data\\ball.bmp");
	glsl_obj_ball_marker->GLSL_INIT_2();


	glsl_obj_poster->Model_Load();  // 函数重载与覆盖 // 
	glsl_obj_poster->LoadGLTextures("Data\\Poster_Al2048.bmp", 1); //  Poster_All.bmp  
	glsl_obj_poster->GLSL_INIT_2(); 
	//
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); //tightly packed
	glPixelStorei(GL_PACK_ALIGNMENT, 1);   //tightly packed
	glEnable(GL_DEPTH_TEST);               /* Hidden surface removal. */  

	screen_obj->InitFBO(mult_camera->WIDTH, mult_camera->HEIGHT);  // 初次初始化 //  
}
// 
void Draw_multiple_obj::Reshape(GLsizei w,GLsizei h)
{
	glViewport(0 , 0 , w , h );    // 自动改变窗口大小  
	window_w = w ; // 
	window_h = h ; // 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if(w<=h){ 
		glFrustum(-Window_size/WIN_DOW_SCALE , Window_size/WIN_DOW_SCALE,-Window_size/WIN_DOW_SCALE*(GLfloat)h/w,Window_size/WIN_DOW_SCALE*(GLfloat)h/w,1.0f/WIN_DOW_SCALE,120.0f);
	}
	else
	{
		glFrustum(-Window_size/WIN_DOW_SCALE*(GLfloat)w/h,Window_size/WIN_DOW_SCALE*(GLfloat)w/h,-Window_size/WIN_DOW_SCALE ,Window_size/WIN_DOW_SCALE,1.0f/WIN_DOW_SCALE,120.0f);
	}
} 
// 
void Draw_multiple_obj::mouse(int botton,int state,int  x,int y )
{
	switch(botton)
	{
	case GLUT_MIDDLE_BUTTON:  
		counter ++ ; // 因为抬起和下降的过程中都会进入这个循环  // 
		std::cout<<counter<<endl ; 
		if (counter==2)
		{
			counter  =  0 ; // 
			if( mult_camera->IF_Square > 0  ) // 如果 mult_camera->IF_Square=1  则 mult_camera->IF_Square=-1；
			{
				if ( menu_display=='5' || menu_display=='4' || menu_display=='0' ) // 如果是Front 则采取直接前后普通拍摄 
				{ // 如果
					glsl_obj_softbody->GLSL_INIT_2();   
				}
				else
				{
					glsl_obj_softbody->GLSL_INIT();   
				}
				if(  menu_display != '0')
				{
					mult_camera->Multiple_View_Set(   mult_camera->PITCH  , mult_camera->GAP , mult_camera->PPI * mult_camera->double_of_pixel  , mult_camera->WIDTH ,mult_camera->HEIGHT  , 1 ) ; //  
					screen_obj->InitFBO( mult_camera->WIDTH ,mult_camera->HEIGHT ); 
				}
			} 
			else // 
			{
				if (menu_display=='5' || menu_display=='4' || menu_display=='0' ) // 如果是Front 则采取直接前后普通拍摄 
				{
					glsl_obj_softbody->GLSL_INIT_2();   // 
				}
				else
				{
					glsl_obj_softbody->GLSL_INIT();    //
				} 
				if(menu_display!='0')
				{
					screen_obj->InitFBO( mult_camera->WIDTH ,mult_camera->HEIGHT ); 
					mult_camera->Multiple_View_Set(  mult_camera->PITCH  , mult_camera->GAP , mult_camera->PPI * mult_camera->double_of_pixel  , mult_camera->WIDTH ,mult_camera->HEIGHT  , 1 ) ; //  2.405303030304  , 12 , 264 , 2048,1536, 1 ) ; // 
				}
			}
		}
		break;
	default: 
		m_baiscobj->mouse(botton,state,x,y) ;
	}
}
#pragma endregion ss 
