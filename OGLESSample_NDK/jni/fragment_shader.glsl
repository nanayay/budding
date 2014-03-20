#version 100
//precision lowp float;
//precision mediump float;
precision highp float;
varying vec4 v_vColor;
void main()
{
    gl_FragColor = v_vColor;
}