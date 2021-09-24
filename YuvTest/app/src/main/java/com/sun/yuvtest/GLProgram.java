package com.sun.yuvtest;

import android.opengl.GLES20;
import android.util.Log;

import java.nio.Buffer;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;

public class GLProgram {
	
	private ByteBuffer _vertice_buffer;
	private ByteBuffer _coord_buffer;
	private boolean isProgBuilt = false;	
	
	private int _positionHandle;
	private int _coordHandle;
	private int  _yhandle;
	private int  _uhandle;	
	private int  _vhandle;	
	
	private int  	_utid;
	private int  	_ytid;
	private int  	_vtid;
	
	private int _video_width;
	private int _video_height;
	
	private int _program;
	private int _textureI = GLES20.GL_TEXTURE0;
	private int _textureII = GLES20.GL_TEXTURE1;
	private int _textureIII = GLES20.GL_TEXTURE2;

	private int _tIindex = 0;
	private int _tIIindex = 1;
	private int _tIIIindex = 2;	
	private String Tag = "GLProgram";
	//private static float[] squareVertices = { -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, }; // fullscreen  
	
	private static float[] _vertices = { -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, }; 
	
	private static float[] coordVertices = { 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, };// whole-texture
//	private static float[] coordVertices = {1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f};//20210712 人脸识别使用此将反转后的镜像反转回来
	  
	private static final String VERTEX_SHADER = "attribute vec4 vPosition;\n" + "attribute vec2 a_texCoord;\n"
	        + "varying vec2 tc;\n" + "void main() {\n" + "gl_Position = vPosition;\n" + "tc = a_texCoord;\n" + "}\n";  
	  
	private static final String FRAGMENT_SHADER = "precision mediump float;\n" + "uniform sampler2D tex_y;\n"
	        + "uniform sampler2D tex_u;\n" + "uniform sampler2D tex_v;\n" + "varying vec2 tc;\n" + "void main() {\n"  
	        + "vec4 c = vec4((texture2D(tex_y, tc).r - 16./255.) * 1.164);\n"  
	        + "vec4 U = vec4(texture2D(tex_u, tc).r - 128./255.);\n"  
	        + "vec4 V = vec4(texture2D(tex_v, tc).r - 128./255.);\n" + "c += V * vec4(1.596, -0.813, 0, 0);\n"  
	        + "c += U * vec4(0, -0.392, 2.017, 0);\n" + "c.a = 1.0;\n" + "gl_FragColor = c;\n" + "}\n";  
	
	GLProgram(int program)
	{
		_program = program;
	}
	
	public boolean isProgramBuilt() {  
	    return isProgBuilt;  
	}  
	  
	public void buildProgram() {  
	    createBuffers(_vertices, coordVertices);  
	    if (_program <= 0) {  
	        _program = createProgram(VERTEX_SHADER, FRAGMENT_SHADER);  
	    }  
	    Log.d(Tag, "_program = " + _program);
	  
	    /* 
	     * get handle for "vPosition" and "a_texCoord" 
	     */  
	    _positionHandle = GLES20.glGetAttribLocation(_program, "vPosition");
	    Log.d(Tag,"_positionHandle = " + _positionHandle);
	    checkGlError("glGetAttribLocation vPosition");  
	    if (_positionHandle == -1) {  
	        throw new RuntimeException("Could not get attribute location for vPosition");
	    }  
	    _coordHandle = GLES20.glGetAttribLocation(_program, "a_texCoord");
	    Log.d(Tag,"_coordHandle = " + _coordHandle);
	    checkGlError("glGetAttribLocation a_texCoord");  
	    if (_coordHandle == -1) {  
	        throw new RuntimeException("Could not get attribute location for a_texCoord");
	    }  
	  
	    /* 
	     * get uniform location for y/u/v, we pass data through these uniforms 
	     */  
	    _yhandle = GLES20.glGetUniformLocation(_program, "tex_y");
	    Log.d(Tag,"_yhandle = " + _yhandle);
	    checkGlError("glGetUniformLocation tex_y");  
	    if (_yhandle == -1) {  
	        throw new RuntimeException("Could not get uniform location for tex_y");
	    }  
	    _uhandle = GLES20.glGetUniformLocation(_program, "tex_u");
	    Log.d(Tag,"_uhandle = " + _uhandle);
	    checkGlError("glGetUniformLocation tex_u");  
	    if (_uhandle == -1) {  
	        throw new RuntimeException("Could not get uniform location for tex_u");
	    }  
	    _vhandle = GLES20.glGetUniformLocation(_program, "tex_v");
	    Log.d(Tag,"_vhandle = " + _vhandle);
	    checkGlError("glGetUniformLocation tex_v");  
	    if (_vhandle == -1) {  
	        throw new RuntimeException("Could not get uniform location for tex_v");
	    }  
	  
	    isProgBuilt = true;  
	}  
	  
	/** 
	 * build a set of textures, one for Y, one for U, and one for V. 
	 */  
	public void buildTextures(Buffer y, Buffer u, Buffer v, int width, int height) {
	    boolean videoSizeChanged = (width != _video_width || height != _video_height);  
	    if (videoSizeChanged) {  
	        _video_width = width;  
	        _video_height = height;  
	        Log.d(Tag,"buildTextures videoSizeChanged: w=" + _video_width + " h=" + _video_height);
	    }  
	  
	    // building texture for Y data  
	    if (_ytid < 0 || videoSizeChanged) {  
	        if (_ytid >= 0) {  
	            Log.d(Tag,"glDeleteTextures Y");
	            GLES20.glDeleteTextures(1, new int[] { _ytid }, 0);
	            checkGlError("glDeleteTextures");  
	        }  
	        // GLES20.glPixelStorei(GLES20.GL_UNPACK_ALIGNMENT, 1);  
	        int[] textures = new int[1];  
	        GLES20.glGenTextures(1, textures, 0);
	        checkGlError("glGenTextures");  
	        _ytid = textures[0];  
	        Log.d(Tag,"glGenTextures Y = " + _ytid);
	    }  
	    GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, _ytid);
	    checkGlError("glBindTexture");  
	    GLES20.glTexImage2D(GLES20.GL_TEXTURE_2D, 0, GLES20.GL_LUMINANCE, _video_width, _video_height, 0,
	            GLES20.GL_LUMINANCE, GLES20.GL_UNSIGNED_BYTE, y);
	    checkGlError("glTexImage2D");  
	    GLES20.glTexParameterf(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MIN_FILTER, GLES20.GL_NEAREST);
	    GLES20.glTexParameterf(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MAG_FILTER, GLES20.GL_LINEAR);
	    GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_S, GLES20.GL_CLAMP_TO_EDGE);
	    GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_T, GLES20.GL_CLAMP_TO_EDGE);
	  
	    // building texture for U data  
	    if (_utid < 0 || videoSizeChanged) {  
	        if (_utid >= 0) {  
	            Log.d(Tag,"glDeleteTextures U");
	            GLES20.glDeleteTextures(1, new int[] { _utid }, 0);
	            checkGlError("glDeleteTextures");  
	        }  
	        int[] textures = new int[1];  
	        GLES20.glGenTextures(1, textures, 0);
	        checkGlError("glGenTextures");  
	        _utid = textures[0];  
	        Log.d(Tag,"glGenTextures U = " + _utid);
	    }  
	    GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, _utid);
	    GLES20.glTexImage2D(GLES20.GL_TEXTURE_2D, 0, GLES20.GL_LUMINANCE, _video_width / 2, _video_height / 2, 0,
	            GLES20.GL_LUMINANCE, GLES20.GL_UNSIGNED_BYTE, u);
	    GLES20.glTexParameterf(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MIN_FILTER, GLES20.GL_NEAREST);
	    GLES20.glTexParameterf(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MAG_FILTER, GLES20.GL_LINEAR);
	    GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_S, GLES20.GL_CLAMP_TO_EDGE);
	    GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_T, GLES20.GL_CLAMP_TO_EDGE);
	  
	    // building texture for V data  
	    if (_vtid < 0 || videoSizeChanged) {  
	        if (_vtid >= 0) {  
	            Log.d(Tag,"glDeleteTextures V");
	            GLES20.glDeleteTextures(1, new int[] { _vtid }, 0);
	            checkGlError("glDeleteTextures");  
	        }  
	        int[] textures = new int[1];  
	        GLES20.glGenTextures(1, textures, 0);
	        checkGlError("glGenTextures");  
	        _vtid = textures[0];  
	        Log.d(Tag,"glGenTextures V = " + _vtid);
	    }  
	    GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, _vtid);
	    GLES20.glTexImage2D(GLES20.GL_TEXTURE_2D, 0, GLES20.GL_LUMINANCE, _video_width / 2, _video_height / 2, 0,
	            GLES20.GL_LUMINANCE, GLES20.GL_UNSIGNED_BYTE, v);
	    GLES20.glTexParameterf(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MIN_FILTER, GLES20.GL_NEAREST);
	    GLES20.glTexParameterf(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MAG_FILTER, GLES20.GL_LINEAR);
	    GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_S, GLES20.GL_CLAMP_TO_EDGE);
	    GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_T, GLES20.GL_CLAMP_TO_EDGE);
	}  
	  
	/** 
	 * render the frame 
	 * the YUV data will be converted to RGB by shader. 
	 */  
	public void drawFrame() {  
	    GLES20.glUseProgram(_program);
	    checkGlError("glUseProgram");  
	  
	    GLES20.glVertexAttribPointer(_positionHandle, 2, GLES20.GL_FLOAT, false, 8, _vertice_buffer);
	    checkGlError("glVertexAttribPointer mPositionHandle");  
	    GLES20.glEnableVertexAttribArray(_positionHandle);
	  
	    GLES20.glVertexAttribPointer(_coordHandle, 2, GLES20.GL_FLOAT, false, 8, _coord_buffer);
	    checkGlError("glVertexAttribPointer maTextureHandle");  
	    GLES20.glEnableVertexAttribArray(_coordHandle);
	  
	    // bind textures  
	    GLES20.glActiveTexture(_textureI);
	    GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, _ytid);
	    GLES20.glUniform1i(_yhandle, _tIindex);
	  
	    GLES20.glActiveTexture(_textureII);
	    GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, _utid);
	    GLES20.glUniform1i(_uhandle, _tIIindex);
	  
	    GLES20.glActiveTexture(_textureIII);
	    GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, _vtid);
	    GLES20.glUniform1i(_vhandle, _tIIIindex);
	  
	    GLES20.glDrawArrays(GLES20.GL_TRIANGLE_STRIP, 0, 4);
	    GLES20.glFinish();
	  
	    GLES20.glDisableVertexAttribArray(_positionHandle);
	    GLES20.glDisableVertexAttribArray(_coordHandle);
	}  
	  
	/** 
	 * create program and load shaders, fragment shader is very important. 
	 */  
	public int createProgram(String vertexSource, String fragmentSource) {
	    // create shaders  
	    int vertexShader = loadShader(GLES20.GL_VERTEX_SHADER, vertexSource);
	    int pixelShader = loadShader(GLES20.GL_FRAGMENT_SHADER, fragmentSource);
	    // just check  
	    Log.d(Tag,"vertexShader = " + vertexShader);
	    Log.d(Tag,"pixelShader = " + pixelShader);
	  
	    int program = GLES20.glCreateProgram();
	    if (program != 0) {  
	        GLES20.glAttachShader(program, vertexShader);
	        checkGlError("glAttachShader");  
	        GLES20.glAttachShader(program, pixelShader);
	        checkGlError("glAttachShader");  
	        GLES20.glLinkProgram(program);
	        int[] linkStatus = new int[1];  
	        GLES20.glGetProgramiv(program, GLES20.GL_LINK_STATUS, linkStatus, 0);
	        if (linkStatus[0] != GLES20.GL_TRUE) {
	            Log.e(Tag,"Could not link program: ", null);
	            Log.e(Tag, GLES20.glGetProgramInfoLog(program), null);
	            GLES20.glDeleteProgram(program);
	            program = 0;  
	        }  
	    }  
	    return program;  
	}  
	  
	/** 
	 * create shader with given source. 
	 */  
	private int loadShader(int shaderType, String source) {
	    int shader = GLES20.glCreateShader(shaderType);
	    if (shader != 0) {  
	        GLES20.glShaderSource(shader, source);
	        GLES20.glCompileShader(shader);
	        int[] compiled = new int[1];  
	        GLES20.glGetShaderiv(shader, GLES20.GL_COMPILE_STATUS, compiled, 0);
	        if (compiled[0] == 0) {  
	            Log.e(Tag,"Could not compile shader " + shaderType + ":", null);
	            Log.e(Tag, GLES20.glGetShaderInfoLog(shader), null);
	            GLES20.glDeleteShader(shader);
	            shader = 0;  
	        }  
	    }  
	    return shader;  
	}  
	  
	/** 
	 * these two buffers are used for holding vertices, screen vertices and texture vertices. 
	 */  
	private void createBuffers(float[] vert, float[] coord) {  
	    _vertice_buffer = ByteBuffer.allocateDirect(vert.length * 4);
	    _vertice_buffer.order(ByteOrder.nativeOrder());
	    _vertice_buffer.asFloatBuffer().put(vert);  
	    _vertice_buffer.position(0);  
	  
	    if (_coord_buffer == null) {  
	        _coord_buffer = ByteBuffer.allocateDirect(coord.length * 4);
	        _coord_buffer.order(ByteOrder.nativeOrder());
	        _coord_buffer.asFloatBuffer().put(coord);  
	        _coord_buffer.position(0);  
	    }  
	}  
	  
	private void checkGlError(String op) {
	    int error;  
	    while ((error = GLES20.glGetError()) != GLES20.GL_NO_ERROR) {
	        Log.e(Tag,"***** " + op + ": glError " + error, null);
	        throw new RuntimeException(op + ": glError " + error);
	    }  
	}  
	  

}
