#pragma region
#include "Angel.h"  /* Angel.h là file tự phát triển (tác giả Prof. Angel), có chứa cả khai báo includes glew và freeglut*/

typedef vec4 point4;
typedef vec4 color4;
using namespace std;
#pragma endregion Thư viện và định nghĩa
#pragma region
void generateGeometry(void);
void initGPUBuffers(void);
void shaderSetup(void);
void display(void);
void keyboard(unsigned char key, int x, int y);
color4 ConvertRGB(int a, int b, int c);
#pragma endregion Prototype
#pragma region
GLuint program, loc_vPosition, loc_vColor, loc_vNormal, model_loc, projection_loc, view_loc;
#pragma endregion Các biến chương trình
#pragma region
vec4 black = color4(0.0, 0.0, 0.0, 1.0);
vec4 red = color4(1.0, 0.0, 0.0, 1.0);
vec4 yellow = color4(1.0, 1.0, 0.0, 1.0);
vec4 green = color4(0.0, 1.0, 0.0, 1.0);
vec4 blue = color4(0.0, 0.0, 1.0, 1.0);
vec4 magenta = color4(1.0, 0.0, 1.0, 1.0);
vec4 pink = color4(1.0, 0.9, 1.0, 1.0);
vec4 cyan = color4(0.0, 1.0, 1.0, 1.0);
vec4 grey = color4(0.25, 0.25, 0.25, 1.0);
vec4 silver = color4(0.75, 0.75, 0.75, 1.0);
vec4 white = color4(1.0, 1.0, 1.0, 1.0);
vec4 redDark = color4(0.65, 0, 0, 1.0);
vec4 yellowLight = color4(1.0, 1.0, 0.75, 1.0);
#pragma endregion Mã màu
#pragma region
const int soDinhHLP = 36;
const int soDinhHinhTru = 144;
const int soDinhCau = 360;
const int soDinhLa = 12;
const int soDinhParabol = 588;
int Index = 0;
point4 points[soDinhHLP + soDinhHinhTru + soDinhLa + soDinhCau + soDinhParabol]; /* Danh sách các đỉnh của các tam giác cần vẽ*/
color4 colors[soDinhHLP + soDinhHinhTru + soDinhLa + soDinhCau + soDinhParabol]; /* Danh sách các màu tương ứng cho các đỉnh trên*/
vec3 normal[soDinhHLP + soDinhHinhTru + soDinhLa + soDinhCau + soDinhParabol];
point4 DinhHLP[8]; /* Danh sách 8 đỉnh của hình lập phương*/
color4 MauHLP[8]; /*Danh sách các màu tương ứng cho 8 đỉnh hình lập phương*/
point4 DinhHtru[26];
color4 MauHtru[26];
point4 DinhHinhCau[62];
color4 MauHinhCau[62];
point4 DinhLa[6];
color4 MauLa[6];
color4 parabol_color[80];
point4 DinhParabol[80];
#pragma endregion Các biến tính toán trong bộ nhớ
#pragma region 
mat4 model, projection, view;
#pragma endregion Model, View, Projection
#pragma region 
GLfloat theta_door[2] = { 0,0 }, theta_model[3] = { 0,0,0 }, z_window = 0, x_truc = 90, Chairmove[2] = { 0,0 }, Rotate_chair = 0, Haxuong = 0.57, QuayCan = 20, status = 0, QuayTruc = 0, Rung_la = -60;
GLfloat camera[3] = { 0,0,0 };
GLfloat but[3] = { -0.02 ,-0.005,-0.02 };
GLfloat light[3] = { 74, 73, 67 };
bool rung = false;
int on = 0;
float index = 0;
#pragma endregion Các biến chức năng
#pragma region 
color4 light_ambient = color4(0.2, 0.2, 0.2, 1.0);
color4 light_diffuse = ConvertRGB(light[0], light[1], light[2]);
color4 light_specular = color4(1.0, 1.0, 1.0, 1.0);
point4 light_position = point4(1, -10, -10, 1.0);
point4 light_position2 = color4(0.0, 0.5, 2.0, 0.0);
point4 light_position3 = color4(10.0, 5, 0, 0.0);
color4 material_ambient(0.2, 0.2, 0.2, 1.0);
color4 material_diffuse(1.0, 0.8, 0.0, 1.0);
color4 material_specular(1.0, 1.0, 1.0, 1.0);
float material_shininess = 100;
#pragma endregion Các biến chiếu sáng
#pragma region 
GLfloat cam_Eye[3] = { 0, 5, 6 };
float QuayCamera[3] = { 270,0,0 };
GLfloat dr = M_PI / 180;
vec4 eye(0, 0.0, -1, 1);
vec4 at(0, 0, -1, 1);
vec4 up(0, 1, 0, 1);
#pragma endregion Các biến điều khiển camera
#pragma region 
void TaoDinhHLP()
{
	// Gán giá trị tọa độ vị trí cho các đỉnh của hình lập phương
	DinhHLP[0] = point4(-0.5, -0.5, 0.5, 1.0);
	DinhHLP[1] = point4(-0.5, 0.5, 0.5, 1.0);
	DinhHLP[2] = point4(0.5, 0.5, 0.5, 1.0);
	DinhHLP[3] = point4(0.5, -0.5, 0.5, 1.0);
	DinhHLP[4] = point4(-0.5, -0.5, -0.5, 1.0);
	DinhHLP[5] = point4(-0.5, 0.5, -0.5, 1.0);
	DinhHLP[6] = point4(0.5, 0.5, -0.5, 1.0);
	DinhHLP[7] = point4(0.5, -0.5, -0.5, 1.0);

	// Gán giá trị màu sắc cho các đỉnh của hình lập phương	
	MauHLP[0] = color4(0.0, 0.0, 0.0, 1.0); // black
	MauHLP[1] = color4(1.0, 0.0, 0.0, 1.0); // red
	MauHLP[2] = color4(1.0, 1.0, 0.0, 1.0); // yellow
	MauHLP[3] = color4(0.0, 1.0, 0.0, 1.0); // green
	MauHLP[4] = color4(0.0, 0.0, 1.0, 1.0); // blue
	MauHLP[5] = color4(1.0, 0.0, 1.0, 1.0); // magenta
	MauHLP[6] = color4(1.0, 1.0, 1.0, 1.0); // white
	MauHLP[7] = color4(0.0, 1.0, 1.0, 1.0); // cyan
}
void TaoDinhHtru()
{
	//hình trụ mặt trên
	int j = 0;
	for (int i = 0; i <= 11; i++) {
		DinhHtru[i] = point4(cos(j * dr), 1, -sin(j * dr), 1.0);
		j = j + 30;
	}
	DinhHtru[12] = point4(0, 1, 0, 1.0);
	//mặt dưới
	j = 0;
	for (int i = 13; i <= 24; i++) {
		DinhHtru[i] = point4(cos(j * dr), -1, -sin(j * dr), 1.0);
		j = j + 30;
	}
	DinhHtru[25] = point4(0, -1, 0, 1.0);
}
void MBHTru(int a, int b, int c, int d) {
	vec4 u = DinhHtru[b] - DinhHtru[a];
	vec4 v = DinhHtru[c] - DinhHtru[b];
	vec3 n = normalize(cross(u, v));
	normal[Index] = n; points[Index] = DinhHtru[a]; Index++;
	normal[Index] = n; points[Index] = DinhHtru[b]; Index++;
	normal[Index] = n; points[Index] = DinhHtru[c]; Index++;

	normal[Index] = n; points[Index] = DinhHtru[a]; Index++;
	normal[Index] = n; points[Index] = DinhHtru[c]; Index++;
	normal[Index] = n; points[Index] = DinhHtru[d]; Index++;
}
void MTHTru(int a[]) {
	vec4 u = DinhHtru[a[1]] - DinhHtru[a[7]];
	vec4 v = DinhHtru[a[4]] - DinhHtru[a[10]];
	vec3 n = normalize(cross(u, v));
	for (int i = 2; i <= 12; i++) {
		normal[Index] = n; points[Index] = DinhHtru[a[0]]; Index++;
		normal[Index] = n; points[Index] = DinhHtru[a[i - 1]]; Index++;
		normal[Index] = n; points[Index] = DinhHtru[a[i]]; Index++;
		if (i == 12) {
			normal[Index] = n; points[Index] = DinhHtru[a[0]]; Index++;
			normal[Index] = n; points[Index] = DinhHtru[a[12]]; Index++;
			normal[Index] = n; points[Index] = DinhHtru[a[1]]; Index++;
		}
	}

}
void HinhTru() {
	int a[] = { 12, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
	int b[] = { 25, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24 };
	//mặt tròn
	MTHTru(a);
	MTHTru(b);
	//thân hình trụ
	for (int i = 1; i <= 11; i++) {
		MBHTru(a[i], a[i + 1], b[i + 1], b[i]);
	}
	MBHTru(a[12], a[1], b[1], b[12]);
}
void TaoDinhHinhCau() {
	float ratioSphere = 1;
	//hình tròn qua tâm
	int j = 0;
	for (int i = 0; i <= 11; i++) {
		DinhHinhCau[i] = point4(cos(j * dr), 0, sin(j * dr), 1.0);
		j = j + 30;
	}
	//hình tròn cách tâm sin 30 độ
	j = 0;
	ratioSphere = cos(30 * dr);
	for (int i = 12; i <= 23; i++) {
		DinhHinhCau[i] = point4(cos(j * dr) * ratioSphere, sin(30 * dr), sin(j * dr) * ratioSphere, 1.0);
		j = j + 30;
	}
	//hình tròn cách tâm sin 60 độ
	j = 0;
	ratioSphere = cos(60 * dr);
	for (int i = 24; i <= 35; i++) {
		DinhHinhCau[i] = point4(cos(j * dr) * ratioSphere, sin(60 * dr), sin(j * dr) * ratioSphere, 1.0);
		j = j + 30;
	}
	//hình tròn cách tâm - sin 30 độ
	j = 0;
	ratioSphere = cos(30 * dr);
	for (int i = 36; i <= 47; i++) {
		DinhHinhCau[i] = point4(cos(j * dr) * ratioSphere, -sin(30 * dr), sin(j * dr) * ratioSphere, 1.0);
		j = j + 30;
	}
	//hình tròn cách tâm - sin 60 độ
	j = 0;
	ratioSphere = cos(60 * dr);
	for (int i = 48; i <= 59; i++) {
		DinhHinhCau[i] = point4(cos(j * dr) * ratioSphere, -sin(60 * dr), sin(j * dr) * ratioSphere, 1.0);
		j = j + 30;
	}
	//đỉnh trên
	DinhHinhCau[60] = point4(0.0, sin(60 * dr), 0.0, 1.0);
	//đỉnh dưới
	DinhHinhCau[61] = point4(0.0, -sin(60 * dr), 0.0, 1.0);
}
void MBHinhCau(int a, int b, int c, int d) {
	vec4 u = DinhHinhCau[b] - DinhHinhCau[a];
	vec4 v = DinhHinhCau[c] - DinhHinhCau[b];
	vec3 n = normalize(cross(u, v));
	normal[Index] = n; points[Index] = DinhHinhCau[a]; Index++;
	normal[Index] = n; points[Index] = DinhHinhCau[b]; Index++;
	normal[Index] = n; points[Index] = DinhHinhCau[c]; Index++;

	normal[Index] = n; points[Index] = DinhHinhCau[a]; Index++;
	normal[Index] = n; points[Index] = DinhHinhCau[c]; Index++;
	normal[Index] = n; points[Index] = DinhHinhCau[d]; Index++;
}
void MTHinhCau(int a[], int pole) {
	vec4 u = DinhHinhCau[a[0]] - DinhHinhCau[a[6]];
	vec4 v = DinhHinhCau[a[4]] - DinhHinhCau[a[9]];
	vec3 n = normalize(cross(u, v));
	for (int i = 1; i <= 11; i++) {
		normal[Index] = n; points[Index] = DinhHinhCau[pole]; Index++;
		normal[Index] = n; points[Index] = DinhHinhCau[a[i - 1]]; Index++;
		normal[Index] = n; points[Index] = DinhHinhCau[a[i]]; Index++;
		if (i == 11) {
			normal[Index] = n; points[Index] = DinhHinhCau[pole]; Index++;
			normal[Index] = n; points[Index] = DinhHinhCau[a[11]]; Index++;
			normal[Index] = n; points[Index] = DinhHinhCau[a[0]]; Index++;
		}
	}
}
void MauDinhHLP(int a, int b, int c, int d)  /*Tạo một mặt hình lập phương = 2 tam giác, gán màu cho mỗi đỉnh tương ứng trong mảng colors*/
{
	vec4 u = DinhHLP[b] - DinhHLP[a];
	vec4 v = DinhHLP[c] - DinhHLP[b];
	vec3 n = normalize(cross(u, v));
	normal[Index] = n; colors[Index] = MauHLP[a]; points[Index] = DinhHLP[a]; Index++;
	normal[Index] = n; colors[Index] = MauHLP[b]; points[Index] = DinhHLP[b]; Index++;
	normal[Index] = n; colors[Index] = MauHLP[c]; points[Index] = DinhHLP[c]; Index++;
	normal[Index] = n; colors[Index] = MauHLP[a]; points[Index] = DinhHLP[a]; Index++;
	normal[Index] = n; colors[Index] = MauHLP[c]; points[Index] = DinhHLP[c]; Index++;
	normal[Index] = n; colors[Index] = MauHLP[d]; points[Index] = DinhHLP[d]; Index++;
}
void MauDinhHtru(int a, int b, int c, int d, int mau)
{
	vec4 u = DinhHtru[b] - DinhHtru[a];
	vec4 v = DinhHtru[c] - DinhHtru[b];
	vec3 n = normalize(cross(u, v));

	normal[Index] = n; colors[Index] = MauHtru[mau]; points[Index] = DinhHtru[a]; Index++;
	normal[Index] = n; colors[Index] = MauHtru[mau]; points[Index] = DinhHtru[b]; Index++;
	normal[Index] = n; colors[Index] = MauHtru[mau]; points[Index] = DinhHtru[c]; Index++;
	normal[Index] = n; colors[Index] = MauHtru[mau]; points[Index] = DinhHtru[a]; Index++;
	normal[Index] = n; colors[Index] = MauHtru[mau]; points[Index] = DinhHtru[c]; Index++;
	normal[Index] = n; colors[Index] = MauHtru[mau]; points[Index] = DinhHtru[d]; Index++;
}
void HinhLapPhuong(void)  /* Sinh ra 12 tam giác: 36 đỉnh, 36 màu*/
{
	MauDinhHLP(1, 0, 3, 2);
	MauDinhHLP(2, 3, 7, 6);
	MauDinhHLP(3, 0, 4, 7);
	MauDinhHLP(6, 5, 1, 2);
	MauDinhHLP(4, 5, 6, 7);
	MauDinhHLP(5, 4, 0, 1);
}
void HinhCau() {
	int a[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
	int b[] = { 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23 };
	int c[] = { 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35 };
	int d[] = { 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47 };
	int e[] = { 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59 };
	//mặt cầu trên tâm
	for (int i = 0; i <= 10; i++) {
		MBHinhCau(a[i], a[i + 1], b[i + 1], b[i]);
	}
	MBHinhCau(a[11], a[0], b[0], b[11]);
	for (int i = 0; i <= 10; i++) {
		MBHinhCau(b[i], b[i + 1], c[i + 1], c[i]);
	}
	MBHinhCau(b[11], b[0], c[0], c[11]);
	//mặt cầu dưới tâm
	for (int i = 0; i <= 10; i++) {
		MBHinhCau(a[i], a[i + 1], d[i + 1], d[i]);
	}
	MBHinhCau(a[11], a[0], d[0], d[11]);
	for (int i = 0; i <= 10; i++) {
		MBHinhCau(d[i], d[i + 1], e[i + 1], e[i]);
	}
	MBHinhCau(d[11], d[0], e[0], e[11]);
	//mặt đỉnh trên
	MTHinhCau(c, 60);
	//mặt đỉnh dưới
	MTHinhCau(e, 61);
}
void TaoDinhLa(void)
{
	DinhLa[0] = point4(0.0, 0.5, 0.0, 1.0);
	DinhLa[1] = point4(-0.25, 0.25, 0.25, 1.0);
	DinhLa[2] = point4(-0.25, -0.25, 0.25, 1.0);
	DinhLa[3] = point4(0.0, -0.5, 0.0, 1.0);
	DinhLa[4] = point4(0.25, -0.25, 0.25, 1.0);
	DinhLa[5] = point4(0.25, 0.25, 0.25, 1.0);

	MauLa[0] = color4(1.0, 0.0, 0.0, 1.0); // đỏ
	MauLa[1] = color4(1.0, 1.0, 0.0, 1.0); // vàng
	MauLa[2] = color4(0.0, 1.0, 0.0, 1.0); // xanh lá
	MauLa[3] = color4(0.0, 0.0, 1.0, 1.0); // xanh lam
	MauLa[4] = color4(1.0, 0.0, 1.0, 1.0); // tím
	MauLa[5] = color4(1.0, 0.0, 0.0, 1.0); // đỏ
}
void MauDinhLa(int a, int b, int c, int mau) {
	vec4 u = DinhLa[a] - DinhLa[b];
	vec4 v = DinhLa[b] - DinhLa[c];
	vec3 n = normalize(cross(u, v));
	normal[Index] = n; colors[Index] = MauLa[mau]; points[Index] = DinhLa[a]; Index++;
	normal[Index] = n; colors[Index] = MauLa[mau]; points[Index] = DinhLa[b]; Index++;
	normal[Index] = n; colors[Index] = MauLa[mau]; points[Index] = DinhLa[c]; Index++;
}
void La() {
	MauDinhLa(0, 1, 5, 7);
	MauDinhLa(5, 1, 2, 7);
	MauDinhLa(2, 4, 5, 7);
	MauDinhLa(4, 2, 3, 7);
}
void TaoDinhParabol() {
	int nums = 0;
	for (int x = -10; x < 10; x += 1) {
		float y = (double)sqrt(25 - x * x);
		DinhParabol[nums] = point4(x, y, 0, 1);
		parabol_color[nums++] = color4(x & 1, 1, 0, 1);
	}
	for (int x = -10; x < 10; x += 1) {
		float y = (double)sqrt(25 - x * x);
		DinhParabol[nums] = point4(x, y, 1, 1);
		parabol_color[nums++] = color4(1, x & 1, 0, 1);
	}
	for (int x = -10; x < 10; x += 1) {
		float y = (double)sqrt(25 - x * x) + 0.5;
		DinhParabol[nums] = point4(x, y, 0, 1);
		parabol_color[nums++] = color4(x & 1, 1, 0, 1);
	}
	for (int x = -10; x < 10; x += 1) {
		float y = (double)sqrt(25 - x * x) + 0.5;
		DinhParabol[nums] = point4(x, y, 1, 1);
		parabol_color[nums++] = color4(1, x & 1, 0, 1);
	}
}
void MauParabol(int a, int b, int c, int d) {
	vec4 u = DinhParabol[b] - DinhParabol[a];
	vec4 v = DinhParabol[c] - DinhParabol[b];
	vec3 n = normalize(cross(u, v));

	normal[Index] = n; colors[Index] = parabol_color[a]; points[Index] = DinhParabol[a]; Index++;
	normal[Index] = n; colors[Index] = parabol_color[a]; points[Index] = DinhParabol[b]; Index++;
	normal[Index] = n; colors[Index] = parabol_color[a]; points[Index] = DinhParabol[c]; Index++;
	normal[Index] = n; colors[Index] = parabol_color[a]; points[Index] = DinhParabol[a]; Index++;
	normal[Index] = n; colors[Index] = parabol_color[a]; points[Index] = DinhParabol[c]; Index++;
	normal[Index] = n; colors[Index] = parabol_color[a]; points[Index] = DinhParabol[d]; Index++;
}
void Parabol() {
	for (int i = 0, j = 1; j < 20; j++, i++) {
		MauParabol(i, j, 20 + j, 20 + i);
	}
	for (int i = 40, j = 41; j < 60; j++, i++) {
		MauParabol(i, j, 20 + j, 20 + i);
	}
	for (int i = 0, j = 1; j < 20; j++, i++) {
		MauParabol(i, j, 40 + j, 40 + i);
	}
	for (int i = 0, j = 1; j < 20; j++, i++) {
		MauParabol(20 + i, 20 + j, 60 + j, 60 + i);
	}
	/*MauParabol(20, 0, 40, 60);
	MauParabol(39, 19, 59, 79);*/
}
void generateGeometry(void)
{
	TaoDinhHLP();
	HinhLapPhuong();
	cout << Index << endl;
	TaoDinhHtru();
	HinhTru();
	cout << Index << endl;
	TaoDinhHinhCau();
	HinhCau();
	cout << Index << endl;
	TaoDinhLa();
	La();
	cout << Index << endl;
	TaoDinhParabol();
	Parabol();
	cout << Index << endl;
}
#pragma endregion Khởi tạo các hình học khác nhau
#pragma region
void initGPUBuffers(void)
{
	// Tạo một VAO - vertex array object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Tạo và khởi tạo một buffer object
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors) + sizeof(normal), NULL, GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors), sizeof(normal), normal);
}
void shaderSetup(void)
{
	// Nạp các shader và sử dụng chương trình shader
	program = InitShader("vshader1.glsl", "fshader1.glsl");   // hàm InitShader khai báo trong Angel.h
	glUseProgram(program);

	// Khởi tạo thuộc tính vị trí đỉnh từ vertex shader
	GLuint loc_vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(loc_vPosition);
	glVertexAttribPointer(loc_vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	GLuint loc_vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(loc_vColor);
	glVertexAttribPointer(loc_vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points)));

	GLuint loc_vNormal = glGetAttribLocation(program, "vNormal");
	glEnableVertexAttribArray(loc_vNormal);
	glVertexAttribPointer(loc_vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points) + sizeof(colors)));

	/* Khởi tạo các tham số chiếu sáng - tô bóng*/
	glUniform4fv(glGetUniformLocation(program, "LightPosition"), 1, light_position);
	glUniform4fv(glGetUniformLocation(program, "LightPosition2"), 1, light_position2);
	glUniform4fv(glGetUniformLocation(program, "LightPosition3"), 1, light_position3);
	glUniform1f(glGetUniformLocation(program, "index"), index);

	model_loc = glGetUniformLocation(program, "Model");
	projection_loc = glGetUniformLocation(program, "Projection");
	view_loc = glGetUniformLocation(program, "View");

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.4, 0.8, 0.47, 1);        /* Thiết lập màu trắng là màu xóa màn hình*/
}
#pragma endregion Tạo bộ nhớ đệm và khởi tạo shader
#pragma region
float DEGtoRAD(float DEG)
{
	return DEG / 360 * 2 * M_PI;
}
void VeHinhLapPhuong(mat4 instance, mat4 matrix)
{
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * matrix * instance);
	glDrawArrays(GL_TRIANGLES, 0, soDinhHLP);    /*Vẽ các tam giác*/
}
void VeHinhTru(mat4 instance, mat4 matrix)
{
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * matrix * instance);
	glDrawArrays(GL_TRIANGLES, soDinhHLP, soDinhHinhTru);    /*Vẽ các tam giác*/
}
void VeLa(mat4 instance, mat4 matrix) {
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * matrix * instance);
	glDrawArrays(GL_TRIANGLES, soDinhHLP + soDinhHinhTru + soDinhCau, soDinhLa);    /*Vẽ các tam giác*/
}
void VeHinhCau(mat4 instance, mat4 matrix) {
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * matrix * instance);
	glDrawArrays(GL_TRIANGLES, soDinhHLP + soDinhHinhTru, soDinhCau);    /*Vẽ các tam giác*/
}
void VeParabol(mat4 instance, mat4 matrix) {
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * matrix * instance);
	glDrawArrays(GL_TRIANGLES, soDinhHLP + soDinhHinhTru + soDinhCau + soDinhLa, soDinhParabol);
}
float Nitpick(int a) {
	return (float)a / 255;
}
color4 ConvertRGB(int a, int b, int c) {
	return color4(Nitpick(a), Nitpick(b), Nitpick(c), 1.0);
}
void FillColor(vec4 color) {
	material_ambient = color;
	material_diffuse = color;
	material_specular = color;
	material_shininess = 100;
	if (on == 1) {
		light[0] = 255;
		light[1] = 255;
		light[2] = 255;
	}
	else if (on == -1) {
		light[0] = 227;
		light[1] = 223;
		light[2] = 163;
	}
	else {
		light[0] = 74;
		light[1] = 73;
		light[2] = 67;
	}
	light_diffuse = ConvertRGB(light[0], light[1], light[2]);
	color4 ambient_product = light_ambient * material_ambient;
	color4 diffuse_product = light_diffuse * material_diffuse;
	color4 specular_product = light_specular * material_specular;
	glUniform4fv(glGetUniformLocation(program, "AmbientProduct"), 1, ambient_product);
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
	glUniform4fv(glGetUniformLocation(program, "SpecularProduct"), 1, specular_product);
	glUniform1f(glGetUniformLocation(program, "Shininess"), material_shininess);
}
void FillColor2(vec4 diffuse, vec4 ambient, vec4 specular, int dobong) {
	material_ambient = ambient;
	material_diffuse = diffuse;
	material_specular = specular;
	material_shininess = dobong;
	if (on == 1) {
		light[0] = 255;
		light[1] = 255;
		light[2] = 255;
	}
	else if (on == -1) {
		light[0] = 227;
		light[1] = 223;
		light[2] = 163;
	}
	else {
		light[0] = 74;
		light[1] = 73;
		light[2] = 67;
	}
	light_diffuse = ConvertRGB(light[0], light[1], light[2]);
	color4 ambient_product = light_ambient * material_ambient;
	color4 diffuse_product = light_diffuse * material_diffuse;
	color4 specular_product = light_specular * material_specular;
	glUniform1f(glGetUniformLocation(program, "Shininess"), material_shininess);
	glUniform4fv(glGetUniformLocation(program, "AmbientProduct"), 1, ambient_product);
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
	glUniform4fv(glGetUniformLocation(program, "SpecularProduct"), 1, specular_product);
}
#pragma endregion Hàm sẵn
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///																											  ///
///									PHẦN THIẾT KẾ MÔ HÌNH PHÒNG LÀM VIỆC								      ///
///																											  ///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma region
mat4 tempModel;
mat4 instance;

void TuongBaoQuanh() {
	//Floor
	material_diffuse = ConvertRGB(212, 146, 59);
	FillColor(material_diffuse);
	instance = Translate(0.0f, -0.51f, 0.0f) * Scale(1.04f, 0.02f, 1.04f);
	VeHinhLapPhuong(instance, mat4());
	//Ceiling
	material_diffuse = ConvertRGB(196, 209, 82);
	FillColor(material_diffuse);
	instance = Translate(0.0f, 0.51f, 0.0f) * Scale(1.04f, 0.02f, 1.04f);
	VeHinhLapPhuong(instance, mat4());
	//BackWall
	material_diffuse = ConvertRGB(204, 200, 194);
	FillColor(material_diffuse);
	instance = Translate(0.0f, 0.0f, 0.51f) * Scale(1.0f, 1.0f, 0.02f);
	VeHinhLapPhuong(instance, mat4());
	//RightWall
	material_diffuse = ConvertRGB(204, 200, 194);
	FillColor(material_diffuse);
	instance = Translate(0.51f, 0.0f, 0.0f) * Scale(0.02f, 1.0f, 1.0f);
	VeHinhLapPhuong(instance, mat4());
	//LeftWall
	instance = Translate(-0.51f, 0.35f, 0.0f) * Scale(0.02f, 0.3f, 0.4f);
	VeHinhLapPhuong(instance, mat4());
	instance = Translate(-0.51f, -0.35f, 0.0f) * Scale(0.02f, 0.3f, 0.4f);
	VeHinhLapPhuong(instance, mat4());
	instance = Translate(-0.51f, 0.0f, -0.35f) * Scale(0.02f, 1.0f, 0.3f);
	VeHinhLapPhuong(instance, mat4());
	instance = Translate(-0.51f, 0.0f, 0.35f) * Scale(0.02f, 1.0f, 0.3f);
	VeHinhLapPhuong(instance, mat4());
	//FrontWall
	//instance = Translate(-0.175f, 0.0f, -0.51f)*Scale(0.65f, 1.0f, 0.02f);
	//VeHinhLapPhuong(instance, mat4());
	//instance = Translate(0.325f, 0.3f, -0.51f)*Scale(0.35f, 0.4f, 0.02f);
	//VeHinhLapPhuong(instance, mat4());
	//Khungcuaravao
	/*material_diffuse = ConvertRGB(56, 54, 52);
	FillColor(material_diffuse);
	instance = Translate(0.325f, 0.09f, -0.51f) * Scale(0.35f, 0.02f, 0.02f);
	VeHinhLapPhuong(instance, mat4());
	instance = Translate(0.162f, -0.21f, -0.51f) * Scale(0.025f, 0.58f, 0.02f);
	VeHinhLapPhuong(instance, mat4());
	instance = Translate(0.487f, -0.21f, -0.51f) * Scale(0.025f, 0.58f, 0.02f);
	VeHinhLapPhuong(instance, mat4());*/
	//Khungcuaso
	instance = Translate(-0.51f, 0.19f, 0.0f) * Scale(0.02f, 0.02f, 0.4f);
	VeHinhLapPhuong(instance, mat4());
	instance = Translate(-0.51f, -0.19f, 0.0f) * Scale(0.02f, 0.02f, 0.4f);
	VeHinhLapPhuong(instance, mat4());
	instance = Translate(-0.51f, 0.0f, -0.195f) * Scale(0.02f, 0.36f, 0.01f);
	VeHinhLapPhuong(instance, mat4());
	instance = Translate(-0.51f, 0.0f, 0.195f) * Scale(0.02f, 0.36f, 0.01f);
	VeHinhLapPhuong(instance, mat4());
	//Cuasobenphai
	material_diffuse = ConvertRGB(255, 252, 250);
	FillColor(material_diffuse);
	instance = Translate(-0.505f, 0.0f, -0.095f) * Scale(0.01f, 0.36f, 0.19f);
	VeHinhLapPhuong(instance, mat4());
}
void Cuabentrai() {
	material_diffuse = ConvertRGB(176, 91, 16);
	FillColor(material_diffuse);
	instance = Translate(0.075f, 0.0f, 0.0f) * Scale(0.155f, 0.58f, 0.01f);
	VeHinhLapPhuong(instance, mat4());
}
void Cuabenphai() {
	material_diffuse = ConvertRGB(176, 91, 16);
	FillColor(material_diffuse);
	instance = Translate(-0.079f, 0.0f, 0.0f) * Scale(0.155f, 0.58f, 0.01f);
	VeHinhLapPhuong(instance, mat4());
}
void CuaSo() {
	material_diffuse = ConvertRGB(255, 252, 250);
	FillColor(material_diffuse);
	instance = Translate(-0.515f, 0.0f, 0.095f) * Scale(0.01f, 0.36f, 0.19f);
	VeHinhLapPhuong(instance, mat4());
}
void tree() {
	material_diffuse = ConvertRGB(13, 122, 53);
	FillColor(material_diffuse);
	for (float i = 0.075; i <= 0.25; i += 0.05f) {
		for (int rtree = 0; rtree < 360; rtree += 45) {
			instance = RotateY(rtree) * Translate(0.0f, i, 0.02f) * RotateX(Rung_la) * Scale(0.03f, 0.15f, 0.02f);
			VeLa(instance, mat4());
		}
	}
	material_diffuse = ConvertRGB(245, 240, 235);
	FillColor(material_diffuse);
	instance = Translate(0.0f, 0.0f, 0.0f) * Scale(0.04f, 0.045f, 0.04f);
	VeHinhTru(instance, mat4());
	material_diffuse = ConvertRGB(125, 66, 7);
	FillColor(material_diffuse);
	instance = Translate(0.0f, 0.13f, 0.0f) * Scale(0.015f, 0.1f, 0.015f);
	VeHinhTru(instance, mat4());
}
void LungGhe() {
	mat4 matrix = Translate(0, -0.33, 0) * RotateX(10) * Translate(0, 0.33, 0);
	material_diffuse = ConvertRGB(252, 247, 248);
	material_ambient = ConvertRGB(148, 78, 40);
	material_specular = ConvertRGB(235, 184, 103);
	FillColor2(material_diffuse, material_ambient, material_specular, 50);
	instance = Translate(0, 0.2, 0) * Scale(0.2, 0.1, 0.05);
	VeHinhTru(instance, matrix);
	instance = Translate(0.05, 0.3, 0) * Scale(0.03, 0.04, 0.03);
	VeHinhTru(instance, matrix);
	instance = Translate(-0.05, 0.3, 0) * Scale(0.03, 0.04, 0.03);
	VeHinhTru(instance, matrix);
	material_diffuse = ConvertRGB(240, 12, 58);
	material_ambient = ConvertRGB(217, 52, 68);
	material_specular = ConvertRGB(237, 119, 150);
	FillColor2(material_diffuse, material_ambient, material_specular, 50);
	instance = Translate(0, 0.4, 0) * Scale(0.2, 0.07, 0.05);
	VeHinhTru(instance, matrix);
	material_diffuse = ConvertRGB(46, 43, 44);
	FillColor(material_diffuse);
	instance = Translate(0.2, 0.2, 0) * Scale(0.1, 0.1, 0.05);
	VeHinhTru(instance, matrix);
	instance = Translate(-0.2, 0.2, 0) * Scale(0.1, 0.1, 0.05);
	VeHinhTru(instance, matrix);
	material_diffuse = ConvertRGB(240, 12, 58);
	FillColor(material_diffuse);
	instance = Translate(0.2, -0.1, 0) * Scale(0.1, 0.2, 0.05);
	VeHinhTru(instance, matrix);
	instance = Translate(-0.2, -0.1, 0) * Scale(0.1, 0.2, 0.05);
	VeHinhTru(instance, matrix);
	instance = Translate(0.0, -0.205, 0) * RotateZ(-90) * Scale(0.1, 0.2, 0.05);
	VeHinhTru(instance, matrix);
	material_diffuse = ConvertRGB(189, 84, 49);
	FillColor(material_diffuse);
	instance = Scale(0.25, 0.25, 0.025);
	VeHinhLapPhuong(instance, matrix);
}
void cangat() {
	model *= RotateX(QuayCan);
	instance = RotateZ(90) * Scale(0.005, 0.1, 0.005);
	VeHinhTru(instance, mat4());
	material_diffuse = ConvertRGB(255, 255, 255);
	FillColor(material_diffuse);
	instance = Translate(0.1, 0, 0.0) * Scale(0.015, 0.015, 0.015);
	VeHinhCau(instance, mat4());

	instance = Translate(0.1, 0, -0.05) * RotateX(90) * Scale(0.005, 0.05, 0.005);
	VeHinhTru(instance, mat4());
	instance = Translate(0.1, 0, -0.1) * RotateX(90) * Scale(0.01, 0.03, 0.01);
	VeHinhTru(instance, mat4());
}
void ThanhGhe() {
	material_diffuse = ConvertRGB(240, 12, 58);
	FillColor(material_diffuse);
	instance = RotateX(90) * Scale(0.3, 0.05, 0.02);
	VeHinhTru(instance, mat4());
	material_diffuse = ConvertRGB(245, 242, 243);
	FillColor(material_diffuse);
	instance = Translate(0.35, -0.01, 0) * Translate(-0.05, 0, 0) * RotateZ(-15) * Translate(0.05, 0, 0) * RotateX(90) * Scale(0.1, 0.04, 0.015);
	VeHinhTru(instance, mat4());
	instance = Translate(0.18, -0.05, 0) * Translate(-0.05, 0, 0) * RotateZ(15) * Translate(0.05, 0, 0) * RotateX(90) * Scale(0.1, 0.04, 0.01);
	VeHinhTru(instance, mat4());
	instance = Translate(-0.03, -0.06, 0) * RotateX(90) * Scale(0.13, 0.04, 0.01);
	VeHinhTru(instance, mat4());
	instance = Translate(-0.35, -0.01, 0) * Translate(0.05, 0, 0) * RotateZ(15) * Translate(-0.05, 0, 0) * RotateX(90) * Scale(0.1, 0.04, 0.015);
	VeHinhTru(instance, mat4());
	instance = Translate(-0.22, -0.02, 0) * Translate(-0.05, 0, 0) * RotateZ(-15) * Translate(0.05, 0, 0) * RotateX(90) * Scale(0.1, 0.04, 0.01);
	VeHinhTru(instance, mat4());
	instance = Translate(-0.15, -0.04, 0) * Translate(-0.05, 0, 0) * RotateZ(-15) * Translate(0.05, 0, 0) * RotateX(90) * Scale(0.025, 0.04, 0.025);
	VeHinhTru(instance, mat4());
	instance = Translate(-0.12, -0.18, 0) * Translate(-0.05, 0, 0) * RotateZ(15) * Translate(0.05, 0, 0) * Scale(0.02, 0.1, 0.015);
	VeHinhTru(instance, mat4());
	instance = Translate(-0.1, -0.26, 0) * RotateX(90) * Scale(0.025, 0.04, 0.025);
	VeHinhTru(instance, mat4());
}
void PhantrenGhe() {
	model *= Translate(0, Haxuong, 0.0) * RotateY(Rotate_chair) * Translate(0.0, 0.0, 0.2);
	tempModel = model;
	LungGhe();
	//Dem ghe
	material_diffuse = ConvertRGB(255, 255, 255);
	FillColor(material_diffuse);
	instance = Translate(0.0, -0.3, -0.24) * RotateX(90) * RotateZ(-90) * Scale(0.3, 0.3, 0.025);
	VeHinhTru(instance, mat4());
	//Gactay
	model *= Translate(-0.32, -0.12, -0.19) * Scale(0.6, 0.7, 0.6) * RotateY(-90);
	ThanhGhe();
	model = tempModel * Translate(0.32, -0.12, -0.19) * Scale(0.6, 0.7, 0.6) * RotateY(-90);
	ThanhGhe();
	//Truc ghe
	material_diffuse = ConvertRGB(242, 237, 237);
	FillColor(material_diffuse);
	model = tempModel * Translate(0.0, -0.42, -0.2);
	instance = Scale(0.05, 0.1, 0.05);
	VeHinhTru(instance, mat4());
	model *= Translate(0.05, 0.0, 0);
	cangat();
}
void PhanChanGhe() {
	material_diffuse = ConvertRGB(242, 237, 237);
	FillColor(material_diffuse);
	instance = Scale(0.05, 0.05, 0.05);
	VeHinhTru(instance, mat4());
	for (int i = 0; i <= 360; i += 75) {
		instance = RotateY(i) * RotateX(-70) * Translate(0, -0.16, 0) * Scale(0.01, 0.2, 0.01);
		VeHinhTru(instance, mat4());
		instance = RotateY(i) * RotateX(-70) * Translate(0, -0.35, 0) * Scale(0.04, 0.04, 0.04);
		VeHinhCau(instance, mat4());
	}
}
void Ghe() {
	model *= Translate(-0.05, -0.65, -0.2);
	PhanChanGhe();
	PhantrenGhe();
}
void Quadiacau() {
	material_diffuse = ConvertRGB(209, 237, 28);
	FillColor(material_diffuse);
	instance = Scale(0.1, 0.02, 0.1);
	VeHinhTru(instance, mat4());
	instance = Translate(0, 0.03, 0) * Scale(0.07, 0.02, 0.07);
	VeHinhTru(instance, mat4());
	instance = Translate(0, 0.1, 0) * Scale(0.03, 0.1, 0.03);
	VeHinhTru(instance, mat4());
	material_diffuse = ConvertRGB(147, 147, 153);
	FillColor(material_diffuse);
	instance = Translate(0.0, 0.5, -0.03) * RotateZ(140) * Scale(0.06, 0.06, 0.06);
	VeParabol(instance, mat4());
	/*instance = Translate(0.1, 0.25, 0) * RotateZ(40) * Scale(0.01, 0.02, 0.01);
	VeHinhTru(instance, mat4());
	instance = Translate(-0.25, 0.6, 0) * RotateZ(40) * Scale(0.01, 0.02, 0.01);
	VeHinhTru(instance, mat4());*/
	material_diffuse = ConvertRGB(22, 79, 201);
	FillColor(material_diffuse);
	instance = Translate(0.00, 0.48, 0) * RotateZ(40) * RotateY(QuayTruc) * Scale(0.27, 0.27, 0.27);
	VeHinhCau(instance, mat4());
}
void sach() {
	instance = Translate(0.015f, 0.0f, 0.0f) * Scale(0.01f, 0.35f, 0.25f);
	VeHinhLapPhuong(instance, mat4());
	instance = Translate(0.0f, 0.00f, -0.12f) * Scale(0.02f, 0.175f, 0.025f);
	VeHinhTru(instance, mat4());
	instance = Translate(-0.015f, 0.0f, 0.0f) * Scale(0.01f, 0.35f, 0.25f);
	VeHinhLapPhuong(instance, mat4());
	material_diffuse = ConvertRGB(207, 212, 203);
	FillColor(material_diffuse);
	instance = Translate(0.0f, 0.0f, 0.0f) * Scale(0.015f, 0.35f, 0.23f);
	VeHinhLapPhuong(instance, mat4());
}
void Tu() {
	mat4 tempmodel = model;
	material_diffuse = ConvertRGB(245, 164, 66);
	FillColor(material_diffuse);
	instance = Translate(0.0f, 0.23f, 0.0f) * Scale(0.35f, 0.02f, 0.25f);
	VeHinhLapPhuong(instance, mat4());
	instance = Translate(0.165f, 0.0f, 0.0f) * Scale(0.02f, 0.45f, 0.25f);
	VeHinhLapPhuong(instance, mat4());
	instance = Translate(-0.165f, 0.0f, 0.0f) * Scale(0.02f, 0.45f, 0.25f);
	VeHinhLapPhuong(instance, mat4());
	instance = Translate(0.0f, -0.23f, 0.0f) * Scale(0.35f, 0.02f, 0.25f);
	VeHinhLapPhuong(instance, mat4());
	instance = Translate(0.0f, -0.07f, 0.0f) * Scale(0.35f, 0.02f, 0.25f);
	VeHinhLapPhuong(instance, mat4());
	material_diffuse = ConvertRGB(232, 226, 183);
	FillColor(material_diffuse);
	instance = Translate(0.0f, 0.0f, 0.115f) * Scale(0.33, 0.46f, 0.02f);
	VeHinhLapPhuong(instance, mat4());
	model = tempmodel * Translate(0.155, 0.08, -0.12f) * RotateY(theta_door[0]) * Scale(1, 0.49, 1);
	Cuabenphai();
	model = tempmodel * Translate(-0.15, 0.08, -0.12f) * RotateY(theta_door[1]) * Scale(1, 0.49, 1);
	Cuabentrai();
	model = tempmodel * Translate(-0.1, -0.188, 0) * Scale(0.13, 0.13, 0.13);
	Quadiacau();
	model = tempmodel * Scale(0.7, 0.35, 0.4);
	tempmodel = model;
	material_diffuse = ConvertRGB(41, 50, 171);
	FillColor(material_diffuse);
	model = tempmodel * Translate(0.2, -0.44, 0.0);
	sach();
	material_diffuse = ConvertRGB(150, 31, 209);
	FillColor(material_diffuse);
	model = tempmodel * Translate(0.155, -0.44, 0.0);
	sach();
	material_diffuse = ConvertRGB(209, 31, 126);
	FillColor(material_diffuse);
	model = tempmodel * Translate(0.11, -0.44, 0.0);
	sach();
	material_diffuse = ConvertRGB(245, 7, 35);
	FillColor(material_diffuse);
	model = tempmodel * Translate(0.065, -0.44, 0.0);
	sach();
	material_diffuse = ConvertRGB(237, 202, 76);
	FillColor(material_diffuse);
	model = tempmodel * Translate(-0.005, -0.44, 0.0) * RotateZ(-9);
	sach();
	material_diffuse = ConvertRGB(217, 104, 28);
	FillColor(material_diffuse);
	model = tempmodel * Translate(-0.14, -0.59, 0.0) * Scale(0.60, 2, 0.75) * RotateY(-90) * RotateZ(90);
	sach();

}
void Tranh() {
	material_diffuse = ConvertRGB(230, 174, 23);
	material_ambient = ConvertRGB(120, 74, 10);
	material_specular = ConvertRGB(235, 184, 103);
	FillColor2(material_diffuse, material_ambient, material_specular, 100);
	instance = Translate(0.0f, 0.165f, 0.0f) * Scale(0.58f, 0.02f, 0.02f);
	VeHinhLapPhuong(instance, mat4());
	instance = Translate(0.0f, -0.165f, 0.0f) * Scale(0.58f, 0.02f, 0.02f);
	VeHinhLapPhuong(instance, mat4());
	instance = Translate(0.3f, 0.0f, 0.0f) * Scale(0.02f, 0.35f, 0.02f);
	VeHinhLapPhuong(instance, mat4());
	instance = Translate(-0.3f, 0.0f, 0.0f) * Scale(0.02f, 0.35f, 0.02f);
	VeHinhLapPhuong(instance, mat4());
	material_diffuse = ConvertRGB(240, 239, 237);
	material_ambient = ConvertRGB(255, 255, 255);
	material_specular = ConvertRGB(255, 255, 255);
	FillColor2(material_diffuse, material_ambient, material_specular, 100);
	instance = Translate(0.0f, 0.0f, 0.0f) * Scale(0.58f, 0.31f, 0.02f);
	VeHinhLapPhuong(instance, mat4());
	material_diffuse = ConvertRGB(112, 230, 135);
	material_ambient = ConvertRGB(8, 102, 8);
	material_specular = ConvertRGB(167, 209, 175);
	FillColor2(material_diffuse, material_ambient, material_specular, 100);
	instance = Translate(0.0f, 0.0f, but[0]) * RotateX(90) * Scale(0.05f, 0.02f, 0.05f);
	VeHinhTru(instance, mat4());
	material_diffuse = ConvertRGB(191, 55, 42);
	material_ambient = ConvertRGB(138, 47, 39);
	material_specular = ConvertRGB(232, 166, 160);
	FillColor2(material_diffuse, material_ambient, material_specular, 100);
	instance = Translate(0.2f, 0.0f, but[1]) * RotateX(90) * Scale(0.05f, 0.02f, 0.05f);
	VeHinhTru(instance, mat4());
	material_diffuse = ConvertRGB(237, 230, 97);
	material_ambient = ConvertRGB(130, 124, 17);
	material_specular = ConvertRGB(217, 212, 128);
	FillColor2(material_diffuse, material_ambient, material_specular, 100);
	instance = Translate(-0.2f, 0.0f, but[2]) * RotateX(90) * Scale(0.05f, 0.02f, 0.05f);
	VeHinhTru(instance, mat4());
}
void NguyenVanViet() {
	tempModel = RotateY(theta_model[1]) * RotateX(theta_model[0]) * RotateZ(theta_model[2]);
	model = tempModel;
	TuongBaoQuanh();
	model = tempModel * Translate(0.0f, 0.0f, z_window);
	CuaSo();
	model = tempModel * Translate(-0.44, -0.4625, -0.30);
	tree();
	model = tempModel * Translate(-0.4, 0.1, 0.45) * Scale(0.5, 0.5, 0.5);
	Tu();
	model = tempModel * Translate(0.49, 0, -0.2) * RotateY(90) * Scale(0.3, 0.3, 0.3);
	Tranh();
	model = tempModel * Translate(Chairmove[0], 0, Chairmove[1]) * Translate(0.0, -0.257, 0.0) * Scale(0.3, 0.3, 0.3);
	Ghe();
	model = mat4();
}
#pragma endregion Nguyễn Văn Việt
#pragma region 
mat4 ctmLoc;
//màn hình máy tính
float yXoayMan1 = 0;
float zXoayMan1 = 0;
float gocXoayThanhTreoX = 0;
float gocXoayThanhTreoY = 0;
vec4 dis = grey;
float displayScreen = 0, displayAppCloseButton = 0;
float move1, move2;
float chuotCoordinate[3], appCoordinate[3], closeCoordinate[3];
float appScaleX = 0.04, appScaleY = 0.04;
bool screenOn = false, appOn = false;
void Man(GLfloat x, GLfloat y, GLfloat z) {
	mat4 matrix = ctmLoc * Translate(x, y, z) * RotateY(10);
	mat4 khungManHinh = RotateY(gocXoayThanhTreoY) * Translate(0.295, 0.26 + yXoayMan1, -0.01 + zXoayMan1) * Scale(0.01, 0.3, 0.01);
	FillColor(black);
	VeHinhLapPhuong(khungManHinh, matrix);

	khungManHinh = RotateY(gocXoayThanhTreoY) * Translate(-0.295, 0.26 + yXoayMan1, -0.01 + zXoayMan1) * Scale(0.01, 0.3, 0.01);
	FillColor(black);
	VeHinhLapPhuong(khungManHinh, matrix);

	khungManHinh = RotateY(gocXoayThanhTreoY) * Translate(0, 0.405 + yXoayMan1, -0.01 + zXoayMan1) * Scale(0.58, 0.01, 0.01);
	FillColor(black);
	VeHinhLapPhuong(khungManHinh, matrix);

	khungManHinh = RotateY(gocXoayThanhTreoY) * Translate(0, 0.115 + yXoayMan1, -0.01 + zXoayMan1) * Scale(0.58, 0.01, 0.01);
	FillColor(black);
	VeHinhLapPhuong(khungManHinh, matrix);

	khungManHinh = RotateY(gocXoayThanhTreoY) * Translate(0, 0.26 + yXoayMan1, zXoayMan1) * Scale(0.6, 0.3, 0.01);
	FillColor(black);
	VeHinhLapPhuong(khungManHinh, matrix);

	mat4 manHinh = RotateY(gocXoayThanhTreoY) * Translate(0, 0.26 + yXoayMan1, -0.01 + zXoayMan1) * Scale(0.58, 0.28, 0.01);
	FillColor(dis);
	VeHinhLapPhuong(manHinh, matrix);

	mat4 chuot = RotateY(gocXoayThanhTreoY) * Translate(0.22 - move1, 0.36 + yXoayMan1 - move2, -0.01 + zXoayMan1) * Scale(0.01, 0.01, displayScreen);
	FillColor(white);
	VeHinhLapPhuong(chuot, matrix);

	mat4 app = RotateY(gocXoayThanhTreoY) * Translate(0, 0.32 + yXoayMan1, -0.01 + zXoayMan1) * Scale(appScaleX, appScaleY, displayScreen - 0.005);
	FillColor(cyan);
	VeHinhLapPhuong(app, matrix);

	mat4 closeButton = RotateY(gocXoayThanhTreoY) * Translate(-0.13, 0.37 + yXoayMan1, -0.01 + zXoayMan1) * Scale(0.04, 0.04, displayAppCloseButton);
	FillColor(red);
	VeHinhLapPhuong(closeButton, matrix);
}
void GiaTreo(GLfloat x, GLfloat y, GLfloat z) {
	mat4 matrix = ctmLoc * Translate(x, y, z) * RotateY(10);
	mat4 giaTreo = RotateY(gocXoayThanhTreoY) * Translate(0, 0.26 + yXoayMan1, 0.025 + zXoayMan1) * Scale(0.24, 0.16, 0.04);
	FillColor(black);
	VeHinhLapPhuong(giaTreo, matrix);

	giaTreo = Translate(0, 0, 0.025) * RotateY(gocXoayThanhTreoY) * RotateX(gocXoayThanhTreoX) * Translate(0, 0.12, 0) * Scale(0.1, 0.24, 0.02);
	FillColor(black);
	VeHinhLapPhuong(giaTreo, matrix);

	giaTreo = Translate(0, 0.01, 0) * Scale(0.24, 0.02, 0.16);
	FillColor(black);
	VeHinhLapPhuong(giaTreo, matrix);
}
float yUpMan2 = 0.04;
float yTateMan2 = -10;
float xTateMan2 = 0;
void Man2(GLfloat x, GLfloat y, GLfloat z) {
	mat4 matrix = ctmLoc * Translate(x, y, z);
	mat4 khungManHinh = RotateX(xTateMan2) * RotateY(yTateMan2) * Translate(0.295, 0.22 + yUpMan2, -0.01) * Scale(0.01, 0.3, 0.01);
	FillColor(black);
	VeHinhLapPhuong(khungManHinh, matrix);

	khungManHinh = RotateX(xTateMan2) * RotateY(yTateMan2) * Translate(-0.295, 0.22 + yUpMan2, -0.01) * Scale(0.01, 0.3, 0.01);
	FillColor(black);
	VeHinhLapPhuong(khungManHinh, matrix);

	khungManHinh = RotateX(xTateMan2) * RotateY(yTateMan2) * Translate(0, 0.365 + yUpMan2, -0.01) * Scale(0.58, 0.01, 0.01);
	FillColor(black);
	VeHinhLapPhuong(khungManHinh, matrix);

	khungManHinh = RotateX(xTateMan2) * RotateY(yTateMan2) * Translate(0, 0.075 + yUpMan2, -0.01) * Scale(0.58, 0.01, 0.01);
	FillColor(black);
	VeHinhLapPhuong(khungManHinh, matrix);

	khungManHinh = RotateX(xTateMan2) * RotateY(yTateMan2) * Translate(0, 0.22 + yUpMan2, 0) * Scale(0.6, 0.3, 0.01);
	FillColor(black);
	VeHinhLapPhuong(khungManHinh, matrix);

	mat4 manHienThi = RotateX(xTateMan2) * RotateY(yTateMan2) * Translate(0, 0.22 + yUpMan2, -0.01) * Scale(0.58, 0.28, 0.01);
	FillColor(dis);
	VeHinhLapPhuong(manHienThi, matrix);
}
void GiaTreo2(GLfloat x, GLfloat y, GLfloat z) {
	mat4 matrix = ctmLoc * Translate(x, y, z);
	mat4 giaTreo = RotateX(xTateMan2) * RotateY(yTateMan2) * Translate(0, 0.25 + yUpMan2, 0.04) * Scale(0.02, 0.02, 0.04);
	FillColor(silver);
	VeHinhLapPhuong(giaTreo, matrix);

	giaTreo = Translate(0, 0.2, 0.04) * Scale(0.03, 0.2, 0.03);
	FillColor(grey);
	VeHinhTru(giaTreo, matrix);

	giaTreo = Translate(0, 0.01, 0) * Scale(0.24, 0.02, 0.16);
	FillColor(grey);
	VeHinhLapPhuong(giaTreo, matrix);
}
void ManHinh(GLfloat x, GLfloat y, GLfloat z) {
	Man(x, y, z);
	GiaTreo(x, y, z);
}
void ManHinh2(GLfloat x, GLfloat y, GLfloat z) {
	Man2(x, y, z);
	GiaTreo2(x, y, z);
}
int tateFan = 0;
void CanhQuat(GLfloat x, GLfloat y, GLfloat z) {
	mat4 matrix = ctmLoc * Translate(x, y, z);
	mat4 canhQuat = RotateZ(tateFan) * Translate(0, 0.025, 0) * Scale(0.03, 0.05, 0.01);
	FillColor(red);
	VeHinhLapPhuong(canhQuat, matrix);

	canhQuat = RotateZ(tateFan + 120) * Translate(0, 0.025, 0) * Scale(0.03, 0.05, 0.01);
	FillColor(red);
	VeHinhLapPhuong(canhQuat, matrix);

	canhQuat = RotateZ(tateFan - 120) * Translate(0, 0.025, 0) * Scale(0.03, 0.05, 0.01);
	FillColor(red);
	VeHinhLapPhuong(canhQuat, matrix);

	mat4 khungQuat = Translate(0, 0, 0.02) * RotateX(90) * Scale(0.06, 0.01, 0.06);
	FillColor(black);
	VeHinhTru(khungQuat, matrix);

	khungQuat = RotateX(90) * Scale(0.06, 0.01, 0.06);
	glUniformMatrix4fv(model_loc, 1, GLU_TRUE, model * matrix * khungQuat);
	FillColor(redDark);
	glDrawArrays(GL_LINE_LOOP, soDinhHLP, soDinhHinhTru);    /*Vẽ các tam giác*/
}
//cây máy tính
void CayMayTinh(GLfloat x, GLfloat y, GLfloat z) {
	mat4 matrix = ctmLoc * Translate(x, y, z);
	mat4 khung = Translate(0, 0.01, 0) * Scale(0.2, 0.02, 0.4);
	FillColor(black);
	VeHinhLapPhuong(khung, matrix);

	khung = Translate(-0.095, 0.2, 0) * Scale(0.01, 0.36, 0.4);
	FillColor(black);
	VeHinhLapPhuong(khung, matrix);

	khung = Translate(0.095, 0.2, 0) * Scale(0.01, 0.36, 0.4);
	glUniformMatrix4fv(model_loc, 1, GLU_TRUE, model * matrix * khung);
	FillColor(red);
	glDrawArrays(GL_LINE_LOOP, 0, soDinhHLP);    /*Vẽ các tam giác*/

	khung = Translate(0, 0.2, 0.195) * Scale(0.2, 0.36, 0.01);
	FillColor(black);
	VeHinhLapPhuong(khung, matrix);

	khung = Translate(0, 0.385, 0) * Scale(0.2, 0.01, 0.4);
	FillColor(black);
	VeHinhLapPhuong(khung, matrix);

	mat4 cucNguon = Translate(-0.04, 0.1, 0.09) * Scale(0.1, 0.16, 0.2);
	FillColor(cyan);
	VeHinhLapPhuong(cucNguon, matrix);

	mat4 cardManHinh = Translate(0, 0.2, -0.17) * Scale(0.18, 0.36, 0.01);
	FillColor(grey);
	VeHinhLapPhuong(cardManHinh, matrix);

	CanhQuat(x + 0, y + 0.32, z - 0.19);
	CanhQuat(x + 0, y + 0.08, z - 0.19);
	CanhQuat(x + 0, y + 0.2, z - 0.19);
}
//bàn phím
void BanPhim(GLfloat x, GLfloat y, GLfloat z) {
	mat4 matrix = ctmLoc * Translate(x, y, z);
	mat4 banPhim = Translate(0, 0.01, -0.2) * Scale(0.3, 0.02, 0.12);
	FillColor(silver);
	VeHinhLapPhuong(banPhim, matrix);

	banPhim = Translate(0, 0, -0.2) * RotateX(-5) * Translate(0, 0.02, 0) * Scale(0.28, 0.02, 0.1);
	FillColor(green);
	VeHinhLapPhuong(banPhim, matrix);
}
//chuột
void Chuot(GLfloat x, GLfloat y, GLfloat z) {
	mat4 matrix = ctmLoc * Translate(x, y, z);
	mat4 thanChuot = Translate(0.25 - move1 * 0.25, 0.02, -0.2 - move2 * 0.25) * Scale(0.08, 0.04, 0.12);
	FillColor(green);
	VeHinhLapPhuong(thanChuot, matrix);
}
int tateLap = 15;
float yRotateLap = -(0.255 - cos(tateLap * dr) * 0.255);
float zRotateLap = sin(tateLap * dr) * 0.255;
void Laptop(GLfloat x, GLfloat y, GLfloat z) {
	mat4 matrix = ctmLoc * Translate(x, y, z) * RotateY(-90);
	mat4 banPhim = Translate(0, 0.01, -0.13) * Scale(0.36, 0.02, 0.26);
	FillColor(grey);
	VeHinhLapPhuong(banPhim, matrix);

	banPhim = Translate(0, 0, -0.12) * RotateX(-5) * Translate(0, 0.02, 0.02) * Scale(0.28, 0.01, 0.12);
	FillColor(green);
	VeHinhLapPhuong(banPhim, matrix);
	//
	mat4 khungManHinh = Translate(0, 0.02, 0) * RotateX(tateLap) * Translate(0.175, 0.12, 0) * Scale(0.01, 0.26, 0.01);
	FillColor(black);
	VeHinhLapPhuong(khungManHinh, matrix);

	khungManHinh = Translate(0, 0.02, 0) * RotateX(tateLap) * Translate(-0.175, 0.12, 0) * Scale(0.01, 0.26, 0.01);
	FillColor(black);
	VeHinhLapPhuong(khungManHinh, matrix);

	khungManHinh = Translate(0, 0.275 + yRotateLap, zRotateLap) * Scale(0.36, 0.01, 0.01);
	FillColor(black);
	VeHinhLapPhuong(khungManHinh, matrix);

	khungManHinh = Translate(0, 0.02, 0) * RotateX(tateLap) * Translate(0, 0.13, 0.01) * Scale(0.36, 0.26, 0.01);
	FillColor(black);
	VeHinhLapPhuong(khungManHinh, matrix);

	mat4 manHienThi = Translate(0, 0.02, 0) * RotateX(tateLap) * Translate(0, 0 + 0.14, 0) * Scale(0.34, 0.22, 0.01);
	FillColor(silver);
	VeHinhLapPhuong(manHienThi, matrix);

	mat4 banLe = Translate(0, 0.02, 0) * RotateZ(90) * Scale(0.02, 0.18, 0.02);
	FillColor(grey);
	VeHinhTru(banLe, matrix);
}
void DenBan(GLfloat x, GLfloat y, GLfloat z) {
	mat4 matrix = ctmLoc * Translate(x, y, z);
	mat4 den = Translate(0, 0.07, 0) * Scale(0.05, 0.05, 0.05);
	FillColor(yellowLight);
	VeHinhTru(den, matrix);

	mat4 chanDen = Scale(0.06, 0.01, 0.06);
	FillColor(black);
	VeHinhTru(chanDen, matrix);
}
float denXoayY1 = 180;
float denXoayZ1 = 15;
float denXoayX2 = -15;
float denXoayZ2 = 0;
void DenTo(GLfloat x, GLfloat y, GLfloat z) {
	mat4 matrix = ctmLoc * Translate(x, y, z);
	mat4 thanDen = Scale(0.15, 0.02, 0.15);
	FillColor(grey);
	VeHinhTru(thanDen, matrix);

	thanDen = Translate(0, 0.6, 0) * Scale(0.02, 0.6, 0.02);
	FillColor(grey);
	VeHinhTru(thanDen, matrix);

	mat4 matrix2 = ctmLoc * Translate(x, y, z) * RotateY(denXoayY1);
	thanDen = Translate(0, 1.2, 0) * Scale(0.04, 0.04, 0.04);
	FillColor(red);
	VeHinhCau(thanDen, matrix2);

	thanDen = Translate(0, 1.2, 0) * RotateZ(denXoayZ1) * Translate(0.07, 0, 0) * Scale(0.1, 0.02, 0.02);
	FillColor(grey);
	VeHinhTru(thanDen, matrix2);

	thanDen = Translate(0, 1.2, 0) * RotateZ(denXoayZ1) * Translate(0.17, 0, 0) * Scale(0.04, 0.04, 0.04);
	FillColor(red);
	VeHinhCau(thanDen, matrix2);

	thanDen = Translate(0, 1.2, 0) * RotateZ(denXoayZ1) * Translate(0.17, 0, 0) * RotateX(denXoayX2) * RotateZ(denXoayZ2) * Translate(0, -0.1, 0) * Scale(0.02, 0.1, 0.02);
	FillColor(grey);
	VeHinhTru(thanDen, matrix2);

	mat4 longDen = Translate(0, 1.2, 0) * RotateZ(denXoayZ1) * Translate(0.17, 0, 0) * RotateX(denXoayX2) * RotateZ(denXoayZ2) * Translate(0, -0.2, 0) * Scale(0.06, 0.04, 0.06);
	FillColor(grey);
	VeHinhTru(longDen, matrix2);

	longDen = Translate(0, 1.2, 0) * RotateZ(denXoayZ1) * Translate(0.17, 0, 0) * RotateX(denXoayX2) * RotateZ(denXoayZ2) * Translate(0, -0.3, 0) * Scale(0.12, 0.12, 0.12);
	glUniformMatrix4fv(model_loc, 1, GLU_TRUE, model * matrix2 * longDen);
	FillColor(red);
	glDrawArrays(GL_TRIANGLES, soDinhHLP + soDinhHinhTru, 288 - soDinhHinhTru);    /*Vẽ các tam giác*/

	mat4 bongDen = Translate(0, 1.2, 0) * RotateZ(denXoayZ1) * Translate(0.17, 0, 0) * RotateX(denXoayX2) * RotateZ(denXoayZ2) * Translate(0, -0.25, 0) * Scale(0.06, 0.06, 0.06);
	FillColor(yellowLight);
	VeHinhCau(bongDen, matrix2);
}
void NguyenDucCuong() {

	ctmLoc = RotateY(theta_model[1]) * RotateX(theta_model[0]) * RotateZ(theta_model[2]) * Scale(0.4, 0.4, 0.4);
	ManHinh(0.31, -0.45, 1.1);
	ManHinh2(-0.3, -0.45, 1.1);
	BanPhim(0, -0.45, 1.1);
	Chuot(-0.5, -0.45, 1.1);
	CayMayTinh(-1.1, -0.45, 1);
	Laptop(-1, -0.45, 0);
	DenBan(-0.75, -0.45, 1);
	DenTo(1, -1.22, 0.9);
}
#pragma endregion Nguyễn Đức Cường
#pragma region
mat4 matrix;
void matBan() {
	// mặt bàn 1

	instance = Scale(0.6f, 0.03f, 0.25f);
	VeHinhLapPhuong(instance, matrix);

	//Mặt bàn 2
	instance = Translate(0.425f, 0, 0.225f) * Scale(0.25f, 0.03, 0.7f);
	VeHinhLapPhuong(instance, matrix);

}
void dechan(float x, float y, float z, mat4 ctm) {
	mat4 instance = ctm * Translate(x, y, z) * Scale(0.03f, 0.015f, 0.03f);
	VeHinhLapPhuong(instance, matrix);

}
void chanBan1(float x, float y, float z) {

	mat4 ctm = Translate(x, y, z);
	instance = ctm * RotateZ(-15) * Translate(0, -0.15f, 0) * Scale(0.03f, 0.3f, 0.03f);
	dechan(-0.07619, -0.2865, 0, ctm);
	VeHinhLapPhuong(instance, matrix);

	instance = ctm * RotateZ(15) * Translate(0, -0.15f, 0) * Scale(0.03f, 0.3f, 0.03f);
	dechan(0.07619, -0.2865, 0, ctm);
	VeHinhLapPhuong(instance, matrix);

	// thanh ngang
	instance = Translate(x, y, z) * Translate(0, -0.2f, 0) * Scale(0.13f, 0.03f, 0.03f);
	VeHinhLapPhuong(instance, matrix);
}
// tủ
//Ngăn léo trên
GLfloat theta[] = { 0,0,0 };
void nganKeo1() {
	// mặt dáy
	material_diffuse = ConvertRGB(150, 122, 78);
	FillColor(material_diffuse);
	mat4 ctm = Translate(theta[0], 0, 0) * Translate(0.425f, -0.07, 0.449f);
	instance = ctm * Scale(0.25f, 0.01f, 0.25f);
	VeHinhLapPhuong(instance, matrix);

	// cạch bên
	instance = ctm * Translate(0, 0.03f, -0.12f) * RotateX(-90) * Scale(0.25f, 0.01f, 0.05f);
	VeHinhLapPhuong(instance, matrix);
	// cạch bên
	instance = ctm * Translate(0, 0.03f, 0.12f) * RotateX(-90) * Scale(0.25f, 0.01f, 0.05f);
	VeHinhLapPhuong(instance, matrix);
	// cạch bên
	instance = ctm * Translate(0.12f, 0.03f, 0) * RotateY(-90) * RotateX(-90) * Scale(0.25f, 0.01f, 0.05f);
	VeHinhLapPhuong(instance, matrix);
	// cạch bên
	instance = ctm * Translate(-0.12f, 0.03f, 0) * RotateY(-90) * RotateX(-90) * Scale(0.25f, 0.01f, 0.05f);
	VeHinhLapPhuong(instance, matrix);
	// hộp đỡ ngăn bàn1
	material_diffuse = ConvertRGB(172, 122, 78);
	FillColor(material_diffuse);
	instance = Translate(0.545f, -0.05, 0.45f) * RotateY(-90) * RotateX(-90) * Scale(0.2f, 0.01f, 0.07f);
	VeHinhLapPhuong(instance, matrix);

	instance = Translate(0.425f, -0.08, 0.45f) * Scale(0.25f, 0.01f, 0.25f);
	VeHinhLapPhuong(instance, matrix);

	instance = Translate(0.425f, -0.05, 0.57f) * RotateX(-90) * Scale(0.25f, 0.01f, 0.07f);
	VeHinhLapPhuong(instance, matrix);

}
//Ngan kéo 2
void khungNganKeo2(mat4 ctm) {// kích thước 250*250*250
	material_diffuse = ConvertRGB(203, 155, 121);
	FillColor(material_diffuse);
	// tấm dáy
	instance = ctm * Scale(0.25f, 0.02f, 0.25f);
	VeHinhLapPhuong(instance, matrix);
	// cạnh ben
	instance = ctm * Translate(0.115f, 0.11f, 0) * Scale(0.02f, 0.21f, 0.25f);
	VeHinhLapPhuong(instance, matrix);
	// mặt sau
	instance = ctm * Translate(0, 0.11f, 0.115f) * RotateY(90) * Scale(0.02f, 0.21f, 0.22f);
	VeHinhLapPhuong(instance, matrix);
	// mặt trước
	instance = ctm * Translate(0, 0.11f, -0.115f) * RotateY(90) * Scale(0.02f, 0.21f, 0.22f);
	VeHinhLapPhuong(instance, matrix);
}
GLint goc = 0;
void canhNganKeo2(mat4 ctm) {
	material_diffuse = ConvertRGB(172, 122, 78);
	FillColor(material_diffuse);
	instance = ctm * Translate(-0.115f, 0.11f, 0.125f) * RotateY(goc) * Translate(0, 0, -0.125f) * Scale(0.02f, 0.21f, 0.25f);
	VeHinhLapPhuong(instance, matrix);
}
void nganKeo2() {

	material_diffuse = ConvertRGB(172, 122, 78);
	FillColor(material_diffuse);
	mat4 ctm = Translate(0.425f, -0.3f, 0.45f);
	khungNganKeo2(ctm);
	canhNganKeo2(ctm);
}

void caiBan() {
	//Cai ban1;
	matBan();
	chanBan1(-0.2f, -0.015f, -0.11f);
	chanBan1(-0.2f, -0.015f, 0.11f);
	chanBan1(0.22f, -0.015f, 0.11f);
	chanBan1(0.22f, -0.015f, -0.11f);

	nganKeo1();

	nganKeo2();
}
// Vẽ điều hòa  
GLfloat gocDieuHoa = 0;

void matSauDH() {
	material_diffuse = ConvertRGB(183, 197, 200);
	FillColor(material_diffuse);
	instance = Translate(0, 0.03, -0.01) * Scale(0.76, 0.39, 0.02);
	VeHinhLapPhuong(instance, matrix);
}
void matTraiDH() {
	material_diffuse = ConvertRGB(183, 197, 200);
	FillColor(material_diffuse);
	instance = Translate(-0.39, 0.03, 0.14) * Scale(0.02, 0.35, 0.3);
	VeHinhLapPhuong(instance, matrix);
}
void matPhaiDH() {
	material_diffuse = ConvertRGB(183, 197, 200);
	FillColor(material_diffuse);
	instance = Translate(0.39, 0.03, 0.14) * Scale(0.02, 0.35, 0.3);
	VeHinhLapPhuong(instance, matrix);
}
void matDuoiDH() {
	material_diffuse = ConvertRGB(183, 197, 200);
	FillColor(material_diffuse);
	instance = Translate(0, -0.15, 0.14) * Scale(0.8, 0.02, 0.3);
	VeHinhLapPhuong(instance, matrix);

}
void matTrenDH() {
	material_diffuse = ConvertRGB(183, 197, 200);
	FillColor(material_diffuse);
	instance = Translate(0, 0.21, 0.13) * Scale(0.8, 0.02, 0.28);
	VeHinhLapPhuong(instance, matrix);
	instance = Translate(0, 0.2, 0.275) * RotateZ(-90) * Scale(0.02, 0.4, 0.02);
	VeHinhTru(instance, matrix);
}
void matTruoc1DH() {
	//material_diffuse = ConvertRGB(172, 0, 78);
	FillColor(red);
	instance = Translate(0, 0.06, 0.28) * Scale(0.76, 0.25, 0.02);
	VeHinhLapPhuong(instance, matrix);
}
void matTruoc2DH() {
	FillColor(yellow);
	instance = Translate(0, -0.15, 0.28) * RotateX(gocDieuHoa) * Translate(0, 0.05, 0) * Scale(0.76, 0.1, 0.02);
	VeHinhLapPhuong(instance, matrix);
}
void dieuHoa() {
	matSauDH();
	matTraiDH();
	matPhaiDH();
	matDuoiDH();
	matTrenDH();
	matTruoc1DH();
	matTruoc2DH();
}
// Vẽ quạt trần

// Vẽ cái trục của quạt trần
void trucquatTran() {
	FillColor(green);
	instance = Translate(0, -0.05, 0) * Scale(0.05, 0.1, 0.02);
	VeHinhTru(instance, matrix);
}
void diemQuayQT() {
	instance = RotateY(theta[1]) * Translate(0, -0.15, 0) * Scale(0.2, 0.03, 0.2);
	VeHinhCau(instance, matrix);
}
// Vẽ các cánh quạt
void canhQT(GLfloat a, GLfloat b, GLfloat c, GLfloat goc_quatTran) {
	FillColor(green);
	//instance = Translate(a, b, c) * RotateY(goc_quatTran) * Scale(0.95, 0.05, 0.2);
	//VeHinhLapPhuong(instance, matrix);
	instance = RotateY(theta[1]) * Translate(a, b, c) * RotateY(goc_quatTran) * Scale(0.35, 0.02, 0.1);
	VeHinhTru(instance, matrix);
}
void quatTran() {

	trucquatTran();
	//matrix = Translate(0, -1, 0) *  Translate(0, 1, 0);
	diemQuayQT();
	canhQT(-0.5, -0.15, 0, 0);
	canhQT(0.23, -0.15, -0.43, -120);
	canhQT(0.23, -0.15, 0.43, 120);
}
void muc1() {
	theta[1] += 1;
	glutPostRedisplay();
}
void muc2() {
	theta[1] += 2;
	glutPostRedisplay();
}
void muc3() {
	theta[1] += 3;
	glutPostRedisplay();
}
void muc_off() {
	theta[1] += 0;
	glutPostRedisplay();
}
// Đồ hồ
GLfloat gioGoc = 0, phutGoc = 0, giayGoc = 0;
void kimGio() {
	FillColor(green);
	instance = RotateZ(gioGoc) * Translate(0.0, 0.10, 0.03) * Scale(0.005, 0.1, 0.005);
	VeHinhTru(instance, matrix);
	instance = RotateZ(gioGoc) * Translate(0, 0.18, 0.03) * Scale(0.015, 0.01, 0.015);
	VeHinhCau(instance, matrix);
	material_diffuse = ConvertRGB(225, 225, 225);
	FillColor(material_diffuse);
	instance = RotateZ(gioGoc) * Translate(0, 0.18, 0.023) * Scale(0.01, 0.01, 0.01);
	VeHinhCau(instance, matrix);
}
void diemQuay() {
	FillColor(black);
	instance = Translate(0, 0, 0.03) * RotateX(90) * Scale(0.01, 0.01, 0.01);
	VeHinhTru(instance, matrix);

}
void kimPhut() {
	FillColor(yellow);
	instance = RotateZ(phutGoc + 6) * Translate(0.0, 0.13, 0.035) * Scale(0.005, 0.12, 0.005);
	VeHinhTru(instance, matrix);
	instance = RotateZ(phutGoc + 6) * Translate(0, 0.225, 0.03) * Scale(0.015, 0.01, 0.015);
	VeHinhCau(instance, matrix);
	material_diffuse = ConvertRGB(225, 225, 225);
	FillColor(material_diffuse);
	instance = RotateZ(phutGoc + 6) * Translate(0, 0.225, 0.023) * Scale(0.01, 0.01, 0.01);
	VeHinhCau(instance, matrix);
}
void kimGiay() {
	FillColor(red);
	instance = RotateZ(giayGoc + 12) * Translate(0.0, 0.15, 0.036) * Scale(0.005, 0.14, 0.005);
	VeHinhTru(instance, matrix);
	instance = RotateZ(giayGoc + 12) * Translate(0, 0.26, 0.031) * Scale(0.015, 0.01, 0.015);
	VeHinhCau(instance, matrix);
	material_diffuse = ConvertRGB(225, 225, 225);
	FillColor(material_diffuse);
	instance = RotateZ(giayGoc + 12) * Translate(0, 0.26, 0.023) * Scale(0.01, 0.01, 0.01);
	VeHinhCau(instance, matrix);
}
void soI(GLfloat x, GLfloat y, GLfloat z, GLint goc) {
	material_diffuse = ConvertRGB(225, 204, 65);
	FillColor(material_diffuse);
	instance = Translate(x, y, z + 0.03) * RotateZ(goc) * Scale(0.004, 0.02, 0.005);
	VeHinhTru(instance, matrix);
	instance = Translate(x, y, z + 0.03) * RotateZ(goc) * Translate(0, 0.018, 0) * RotateZ(-90) * Scale(0.003, 0.01, 0.005);
	VeHinhTru(instance, matrix);
	instance = Translate(x, y, z + 0.03) * RotateZ(goc) * Translate(0, -0.018, 0) * RotateZ(-90) * Scale(0.003, 0.01, 0.005);
	VeHinhTru(instance, matrix);
}
void soX(GLfloat x, GLfloat y, GLfloat z) {
	material_diffuse = ConvertRGB(225, 204, 65);
	FillColor(material_diffuse);
	instance = Translate(x, y, z + 0.03) * RotateZ(-40) * Scale(0.004, 0.025, 0.005);
	VeHinhTru(instance, matrix);
	instance = Translate(x, y, z + 0.03) * RotateZ(40) * Scale(0.004, 0.025, 0.005);
	VeHinhTru(instance, matrix);
}
void soIII() {

	soI(-0.335, 0, 0, 0);
	soI(-0.36, 0, 0, 0);
	soI(-0.31, 0, 0, 0);
}
void soVI() {

	soI(0.02, -0.335, 0, -20);
	soI(0, -0.335, 0, 20);
	soI(-0.025, -0.335, 0, 0);
}
void soIX() {

	soI(0.335, 0, 0, 0);
	soX(0.30, 0, 0);
}

void soXII() {
	soX(0.02, 0.335, 0);
	soI(-0.02, 0.335, 0, 0);
	soI(-0.05, 0.335, 0, 0);
}
void timer2(int value) {
	giayGoc += 0.05; //6 dộ = 1 giây (360 độ / 60 giây)
	if (giayGoc >= 360) //khi kim giây quay dược 1 vòng
	{
		phutGoc += 6;
		//velai();
		gioGoc += 2; //giờ quay 0.5 dộ = 1/720 giây
		giayGoc = 0;

		if (phutGoc >= +360) {
			phutGoc = 0;

			if (gioGoc >= +360) {
				gioGoc = 0;
			}
		}
	}
	glutPostRedisplay();
	glutTimerFunc(360000, timer2, 0);
}
void dongHo() {
	kimGiay();
	kimGio();
	kimPhut();
	diemQuay();
	glutTimerFunc(1000, timer2, 0);
	// số 1
	soXII();
	soI(-0.2, 0.27, 0, 40);
	soI(-0.28, 0.16, 0, 60);
	soIII();
	soI(-0.2, -0.27, 0, -40);
	soI(-0.28, -0.16, 0, -60);
	soVI();
	soI(0.2, -0.27, 0, 40);
	soI(0.28, -0.16, 0, 60);
	soIX();
	soI(0.2, 0.27, 0, -40);
	soI(0.28, 0.16, 0, -60);
}

void DoThiHuyen() {
	// cai ban
	tempModel = RotateY(theta_model[1]) * RotateX(theta_model[0]) * RotateZ(theta_model[2]) * Scale(1, 1, 1);
	material_diffuse = ConvertRGB(203, 155, 121);
	FillColor(material_diffuse);
	matrix = tempModel * Translate(0.05, -0.2, 0.375) * RotateY(-180);
	caiBan();

	// dieu hoa
	matrix = tempModel * Translate(0.5, 0.32, 0) * RotateY(270) * Scale(0.4, 0.4, 0.4);
	dieuHoa();

	// quạt trần
	matrix = tempModel * Translate(0, 0.49, 0) * Scale(0.35, 0.35, 0.35);
	quatTran();
	// đồng hòa
	matrix = tempModel * Translate(0, 0.2, 0.48) * Scale(0.3, 0.3, 0.3);
	dongHo();

}
#pragma endregion Nguyễn Thị Huyền
#pragma region
float yDown = 0;
GLfloat radius = 1, thetaCam = 180 * dr, phi = 0;
GLfloat l = -0.01, r = 0.01;
GLfloat bottom = -0.01, top = 0.01;
GLfloat zNear = 0.03, zFar = 10;
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	model = RotateY(theta_model[1]) * RotateX(theta_model[0]) * RotateZ(theta_model[2]);
	NguyenVanViet();
	NguyenDucCuong();
	DoThiHuyen();

	at = vec4(cosf(DEGtoRAD(QuayCamera[0])), 0, cosf(DEGtoRAD(QuayCamera[2])), 0) + eye;
	mat4 view = RotateX(QuayCamera[1]) * LookAt(eye, at, up);
	glUniformMatrix4fv(view_loc, 1, GL_TRUE, view);
	mat4 p = Frustum(l, r, bottom, top, zNear, zFar);
	glUniformMatrix4fv(projection_loc, 1, GL_TRUE, p);

	glViewport(0, 0, 1000, 1000);
	glutPostRedisplay();
	glutSwapBuffers();
}
void RungLa(int value) {
	if (rung == true) {
		if (Rung_la >= -55)
			Rung_la = -60;
		Rung_la += 0.1;
	}
	glutPostRedisplay();
	glutTimerFunc(1, RungLa, 0);
}
void timer(int value) {
	//Viet
	if (status == 1) {
		if (theta_door[0] >= -120) {
			theta_door[0] -= 5;
			theta_door[1] += 5;
		}
		glutPostRedisplay(); // Yêu cầu làm mới lại màn hình
		glutTimerFunc(100, timer, 0); // Đặt hàm gọi lại sau 0.5 giây
	}
	if (status == -1)
	{
		if (theta_door[0] <= 0) {
			theta_door[0] += 5;
			theta_door[1] -= 5;
		}
		glutPostRedisplay(); // Yêu cầu làm mới lại màn hình
		glutTimerFunc(100, timer, 0); // Đặt hàm gọi lại sau 0.5 giây
	}
}
void spinearth(void) {
	QuayTruc += 0.1;
	if (QuayTruc > 360) {
		QuayTruc -= 360;
	}
	glutPostRedisplay();
}
bool cuongControl = false;
bool huyenControl = false;
bool vietControl = false;
void keyboard(unsigned char key, int x, int y)
{
	// keyboard handler
	vec3 cam_forward = normalize(vec3(cosf(DEGtoRAD(QuayCamera[0]))
		* abs(cosf(DEGtoRAD(QuayCamera[1]))), -sinf(DEGtoRAD(QuayCamera[1])),
		cosf(DEGtoRAD(QuayCamera[2])) * abs(cosf(DEGtoRAD(QuayCamera[1])))));
	vec3 cam_right = normalize(cross(cam_forward, up));
	vec4 cam_up = normalize(cross(cam_right, cam_forward));

	if (cuongControl == true) {
		switch (key) {
		case 'u':
			// một số lệnh 
			if (gocXoayThanhTreoX >= -30) {
				gocXoayThanhTreoX -= 5;
				yXoayMan1 = -(0.24 - cos(gocXoayThanhTreoX * dr) * 0.24);
				zXoayMan1 = sin(gocXoayThanhTreoX * dr) * 0.24;
			}
			glutPostRedisplay();
			break;
		case 'U':
			// một số lệnh 
			if (gocXoayThanhTreoX < 5) {
				gocXoayThanhTreoX += 5;
				yXoayMan1 = -(0.24 - cos(gocXoayThanhTreoX * dr) * 0.24);
				zXoayMan1 = sin(gocXoayThanhTreoX * dr) * 0.24;
			}
			glutPostRedisplay();
			break;
		case 'i':
			// một số lệnh 
			if (gocXoayThanhTreoY > -30) {
				gocXoayThanhTreoY -= 5;
			}
			glutPostRedisplay();
			break;
		case 'I':
			// một số lệnh 
			if (gocXoayThanhTreoY < 30) {
				gocXoayThanhTreoY += 5;
			}
			glutPostRedisplay();
			break;
		case 'e':
			// một số lệnh 
			dis = blue;
			displayScreen = 0.02;
			screenOn = true;
			//
			appCoordinate[0] = 0;
			appCoordinate[1] = 0.32 + yXoayMan1;
			appCoordinate[2] = -0.01 + zXoayMan1;
			closeCoordinate[0] = -0.13;
			closeCoordinate[1] = 0.37 + yXoayMan1;
			closeCoordinate[2] = -0.01 + zXoayMan1;
			//
			glutPostRedisplay();
			break;
		case 'E':
			// một số lệnh 
			dis = grey;
			displayScreen = 0;
			displayAppCloseButton = 0;
			screenOn = false;
			glutPostRedisplay();
			break;
		case 'l':
			// một số lệnh 
			if (tateLap >= -85) {
				tateLap -= 5;
				yRotateLap = -(0.255 - cos(tateLap * dr) * 0.255);
				zRotateLap = sin(tateLap * dr) * 0.255;
				glutPostRedisplay();
			}
			break;
		case 'L':
			// một số lệnh 
			if (tateLap <= 40) {
				tateLap += 5;
				yRotateLap = -(0.255 - cos(tateLap * dr) * 0.255);
				zRotateLap = sin(tateLap * dr) * 0.255;
				glutPostRedisplay();
			}
			break;
		case 'x':
			// một số lệnh 
			if (yUpMan2 <= 0.1) {
				yUpMan2 += 0.02;
				glutPostRedisplay();
			}
			break;
		case 'X':
			// một số lệnh 
			if (yUpMan2 >= 0) {
				yUpMan2 -= 0.02;
				glutPostRedisplay();
			}
			break;
		case 'z':
			// một số lệnh 
			if (yTateMan2 <= 30) {
				yTateMan2 += 5;
				glutPostRedisplay();
			}
			break;
		case 'Z':
			// một số lệnh 
			if (yTateMan2 >= -30) {
				yTateMan2 -= 5;
				glutPostRedisplay();
			}
			break;
		case 'b':
			//một số lệnh 
			if (xTateMan2 <= -5) {
				xTateMan2 += 5;
				glutPostRedisplay();
			}
			break;
		case 'B':
			// một số lệnh 
			if (xTateMan2 >= -15) {
				xTateMan2 -= 5;
				glutPostRedisplay();
			}
			break;
		case 'n':
			denXoayY1 += 5;
			glutPostRedisplay();
			break;
		case 'N':
			denXoayY1 -= 5;
			glutPostRedisplay();
			break;
		case 'm':
			denXoayZ1 += 5;
			glutPostRedisplay();
			break;
		case 'M':
			denXoayZ1 -= 5;
			glutPostRedisplay();
			break;
		case 'j':
			denXoayZ2 += 5;
			glutPostRedisplay();
			break;
		case 'J':
			denXoayZ2 -= 5;
			glutPostRedisplay();
			break;
		case 'k':
			denXoayX2 -= 5;
			glutPostRedisplay();
			break;
		case 'K':
			denXoayX2 += 5;
			glutPostRedisplay();
			break;
		}
	}
	if (huyenControl == true) {
		switch (key) {
		case 'o':
			theta[0] += -0.05;
			if (theta[0] <= -0.15f) theta[0] = -0.15;
			glutPostRedisplay();
			break;
		case 'O':
			theta[0] -= -0.05;
			if (theta[0] >= 0) theta[0] = 0;
			glutPostRedisplay();
			break;
		case 'u':
			goc += 5;
			if (goc >= 90) goc = 90;
			glutPostRedisplay();
			break;
		case 'U':
			goc -= 5;
			if (goc <= 0) goc = 0;
			glutPostRedisplay();
			break;
			//quạt trần
		case '3':
			glutIdleFunc(muc3);
			rung = true;
			break;
		case '2':
			glutIdleFunc(muc2);
			rung = true;
			break;
		case '1':
			glutIdleFunc(muc1);
			rung = false;
			break;
		case '0':
			glutIdleFunc(muc_off);
			rung = false;
			break;
			// quay canh dieu hoa
		case 'i':
			gocDieuHoa += 5;
			if (gocDieuHoa > 120) gocDieuHoa = 120;
			glutPostRedisplay();
			break;
		case 'I':
			gocDieuHoa -= 5;
			if (gocDieuHoa < 0) gocDieuHoa = 0;
			glutPostRedisplay();
			break;
			//Hết lenh cua huyen
		case 'R':
			// một số lệnh 
			if (tateLap >= -85) {
				tateLap -= 5;
				yRotateLap = -(0.255 - cos(tateLap * dr) * 0.255);
				zRotateLap = sin(tateLap * dr) * 0.255;
				glutPostRedisplay();
			}
			break;
		case 'v':
			yDown -= 0.1;
			glutPostRedisplay();
			break;
		case ']':
			while (theta_door[1] < 90 && theta_door[0] > -90) {
				theta_door[0] -= 1;
				theta_door[1] += 1;
				glutPostRedisplay();
			}
			break;
		case 'c':
			while (theta_door[1] >= 0 && theta_door[0] <= 0) {
				theta_door[0] += 1;
				theta_door[1] -= 1;
				glutPostRedisplay();
			}
			break;
		case '.':
			if (z_window >= -0.19) {
				z_window -= 0.01f;
			}
			glutPostRedisplay();
			break;
		case '/':
			if (z_window < 0.0f) {
				z_window += 0.01f;
			}
			glutPostRedisplay();
			break;
		}
	}
	if (vietControl == true) {
		switch (key) {
		case 'O':
			on = true;
			break;
		case 'k':
			Rotate_chair += 5;
			glutPostRedisplay();
			break;
		case 'K':
			Rotate_chair -= 5;
			glutPostRedisplay();
			break;
		case 'l':
			if (Haxuong >= 0.5f) {
				Haxuong -= 0.005;
				QuayCan = -20;
			}
			else QuayCan = 20;
			glutPostRedisplay();
			break;
		case 'L':
			if (Haxuong < 0.57f) {
				Haxuong += 0.005;
				QuayCan = -20;
			}
			else QuayCan = 20;
			glutPostRedisplay();
			break;
		case '1':
			if (Chairmove[0] <= 0.35) {
				Chairmove[0] += 0.005;
			}
			glutPostRedisplay();
			break;
		case '!':
			if (Chairmove[0] >= -0.35) {
				Chairmove[0] -= 0.005;
			}
			glutPostRedisplay();
			break;
		case '2':
			if (Chairmove[1] <= 0.35) {
				Chairmove[1] += 0.005;
			}
			glutPostRedisplay();
			break;
		case '@':
			if (Chairmove[1] >= -0.35) {
				Chairmove[1] -= 0.005;
			}
			glutPostRedisplay();
			break;
		case 'm':
			status = 1;
			timer(0);
			break;
		case 'M':
			status = -1;
			timer(0);
			break;
		case 'b':
			if (z_window >= -0.19) {
				z_window -= 0.01f;
				index += 0.02;
				glUniform1f(glGetUniformLocation(program, "index"), index);
			}
			glutPostRedisplay();
			break;
		case 'B':
			if (z_window < 0.0f) {
				z_window += 0.01f;
				index -= 0.02;
				glUniform1f(glGetUniformLocation(program, "index"), index);
			}
			glutPostRedisplay();
			break;
		}
	}
	if (cuongControl == false || huyenControl == false || vietControl == false) {
		switch (key) {
		case 'C':
			cuongControl = true;
			huyenControl = false;
			vietControl = false;
			break;
		case 'H':
			cuongControl = false;
			huyenControl = true;
			vietControl = false;
			break;
		case 'V':
			cuongControl = false;
			huyenControl = false;
			vietControl = true;
			break;
		case 'w': zNear *= 1.1; zFar *= 1.1; break;
		case 's': zNear *= 0.9; zFar *= 0.9; break;
		case 'f':
			QuayCamera[0] += 10 * 0.1;
			QuayCamera[2] += 10 * 0.1;
			break;
		case 'h':
			QuayCamera[0] -= 10 * 0.1;
			QuayCamera[2] -= 10 * 0.1;
			break;
		case 't':
			QuayCamera[1] -= 10 * 0.1;
			if (QuayCamera[1] <= -90) QuayCamera[1] = -90;
			break;
		case 'g':
			QuayCamera[1] += 10 * 0.1;
			if (QuayCamera[1] >= 90) QuayCamera[1] = 90;
			break;
		case 'a':
			eye = eye - cam_right * 0.1;
			break;
		case 'd':
			eye = eye + cam_right * 0.1;
			break;
		}
	}
}
bool Hover(float mouse[], float button[]) {
	int check = 0;
	for (int i = 0; i <= 2; i++) {
		if (mouse[i] <= button[i] + 0.02 && mouse[i] >= button[i] - 0.02) {
			check++;
		}
	}
	if (check == 3) return true;
	return false;
}
void MouseClick(int button, int state, int x, int y) {
	bool appHover = Hover(chuotCoordinate, appCoordinate);
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && appHover == true && screenOn == true) {
		appScaleX = 0.3;
		appScaleY = 0.14;
		displayAppCloseButton = 0.02;
		appOn = true;
		glutPostRedisplay();
	}
	bool closeHover = Hover(chuotCoordinate, closeCoordinate);
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && closeHover == true && screenOn == true && appOn == true) {
		appScaleX = 0.04;
		appScaleY = 0.04;
		displayAppCloseButton = 0;
		appOn = false;
		glutPostRedisplay();
	}
	if (vietControl == true) {
		if (state == GLUT_DOWN) {
			switch (button) {
			case GLUT_LEFT_BUTTON: but[1] = -0.005, but[2] = -0.02, but[0] = -0.02; on = 0; break;
			case GLUT_MIDDLE_BUTTON: but[0] = -0.005, but[2] = -0.02, but[1] = -0.02; on = 1; break;
			case GLUT_RIGHT_BUTTON: but[2] = -0.005, but[0] = -0.02, but[1] = -0.02; on = -1; break;
			}
		}
	}
}
void MouseMove(int x, int y) {
	if (x > 0 && x < 600 && y > 0 && y < 600) {
		move1 = x * 0.0007;
		move2 = y * 0.0003;
		chuotCoordinate[0] = 0.22 - move1;
		chuotCoordinate[1] = 0.36 + yXoayMan1 - move2;
		chuotCoordinate[2] = -0.01 + zXoayMan1;
		appCoordinate[0] = 0;
		appCoordinate[1] = 0.32 + yXoayMan1;
		appCoordinate[2] = -0.01 + zXoayMan1;
		glutPostRedisplay();
	}
}
void timer3(int value) {
	if (screenOn == true) {
		tateFan += 5;
		glutPostRedisplay();
	}
	glutTimerFunc(100, timer3, 0);
}
#pragma endregion Phần hiển thị và thao tác chức năng
int main(int argc, char** argv)
{
	// main function: program starts here
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(1000, 900);
	glutInitWindowPosition(300, 0);
	glutCreateWindow("Drawing a Cube");

	glewInit();

	generateGeometry();
	initGPUBuffers();
	shaderSetup();

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(spinearth);
	glutTimerFunc(0, timer2, 0);
	glutTimerFunc(0, timer3, 0);
	glutTimerFunc(0, RungLa, 0);
	glutMouseFunc(MouseClick);
	glutMotionFunc(MouseMove);
	glutMainLoop();
}
