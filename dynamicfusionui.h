#include <iostream>
#define _CRT_SECURE_NO_WARNINGS
#define  GLEW_STATIC
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <vector>
#include <Windows.h>
#include <iostream>
#include "glsl_loader.h"
#pragma once 
#ifndef __DRAC_HEADER__
#define __DRAC_HEADER__


// #include "Cg_Loader.h"
#include "time_fps.h"
#include "glsl_loader.h"
#include "txtread.h" 
#include "Screen_Shot.h"


class Draw_multiple_obj 
{
public:
	// 
	int iteration_of_display; 
	int Flag__GRASP  ; // 
	int Grasped_Vertex_ID; 
	int Organ_num     ; //  
	// 
	Draw_multiple_obj();
	~Draw_multiple_obj();
	//
	int  Draw_multiple_obj::Organ_Indentification( float x , float y ,float z  ) ;  
	void Draw_multiple_obj::Triangle_Primitive_Determine ( Octree* tmp  )  ; 

	void        Draw_multiple_obj::utility_show_glm_mat4( string src_name , glm::mat4 src) ; 
	glm::mat4   Draw_multiple_obj::Get_mat4_from_float_Fcn ( GLfloat * src )  ; 


	float3 Draw_multiple_obj::Vec_rotation_2 ( float3 vec_in , GLfloat * Rotation_4_by_4 ) ; 
	float3 Draw_multiple_obj::Vec_rotation ( float3 vec_in , GLfloat * Rotation_4_by_4 ) ;

	double        ** Organ  ; 
	int           **Denotion_texture_remapping  ;  
	// 
	GLdouble        Orth_Window ; 
	// 
	vector<vector<string>> strArray  ;   
	int Draw_multiple_obj::Organ_Indentification(int vertex_id); // 重载函数 // 
	//  下面这些可用可不用 // 
	int ** Face_Limit_Rectangle ;  
	int ** Face_Limit_Hexagon ;  

	int             counter     ; 

	GLuint          texture[8]  ;
	GLfloat       * Matrix      ;  // 一个临时变量 
	Obj_control   * m_baiscobj  ;  // 
	GLfloat         Window_size ;  //
	float           WIN_DOW_SCALE ;  
	time_fps      * fps_obj    ;
	txt_Draw      * txt_obj     ; 
	
	GLSL_Loader   * glsl_obj_softbody    ;
	GLSL_Loader   * glsl_obj_ball_marker ;
	GLSL_Loader   * glsl_obj_cutter  ; //
	GLSL_Loader   * glsl_obj_poster  ; // 
	// 
	SCREEN_SHOT   * screen_obj  ; 
	// 
	int             glsl_Flag   ; 
	int            menu_display ;

	Multiple_View     * mult_camera  ;

	static void checkForCgError(const char* situation ) ;

	void Draw_multiple_obj:: Idle() ; 
	void Draw_multiple_obj:: Load_Pipeline(float center_x , float center_y , float window_x , float window_y, float pitch, float pitchy, float backfront_flag ,GLdouble *clip_plane  )    ; 
	void Draw_multiple_obj:: Disable_PipeLine()  ;  // 

	void Draw_multiple_obj:: Load_Pipeline2(); 
	void Draw_multiple_obj:: Disable_PipeLine2();  // 

	void Draw_multiple_obj:: Load_multi_obj() ;
	void Draw_multiple_obj:: InitGL ( GLvoid )   ;  // Create Some Everyday Functions 
	void Draw_multiple_obj:: myDisplay() ;
	void Draw_multiple_obj:: Reshape(GLsizei w,GLsizei h);
	void Draw_multiple_obj:: mouse(int botton,int state,int  x,int y ) ;
	void Draw_multiple_obj:: render() ;
	void Draw_multiple_obj:: renderSingle_Lens(); 
	void Draw_multiple_obj:: Keyboard(unsigned char key, int x, int y) ;
	float Draw_multiple_obj::getHeight(float x, float y) ; 

	void Draw_multiple_obj::render_back_surface  () ;
	void Draw_multiple_obj::render_Front_surface () ;

	void Draw_multiple_obj::render_new () ;
	void Draw_multiple_obj::render_single_viewport(int i, int j ) ;

	void Draw_multiple_obj::render_multiple_iv_front() ;
	void Draw_multiple_obj::render_multiple_iv_back() ;

	void Draw_multiple_obj::Projection_Setup()  ; 
	
	void Draw_multiple_obj::render_glsl() ; 

	void Draw_multiple_obj::Load_csv( ) ; 
	
	
	
	
protected:

private:
	int  window_w , window_h ;
	bool Full_Screen_Flag ;

};


#endif // 
