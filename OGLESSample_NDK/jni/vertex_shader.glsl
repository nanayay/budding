#version 100
attribute vec4 a_vPosition;
attribute vec4 a_vColor;
varying vec4 v_vColor;

void main()
{
    gl_Position = a_vPosition;
    v_vColor = a_vColor;
}