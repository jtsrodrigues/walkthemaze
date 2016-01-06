#include "labirinto.h"

#define ID_TEXTURA_CUBOS          0
#define ID_TEXTURA_CHAO           1
extern int flag = 0;
Porta portas;
FimMundo fimMundo;
Barril barris;
void material(enum tipo_material mat)
{
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient[mat]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse[mat]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular[mat]);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess[mat]);
}


void desenhaSolo(GLuint texID){
#define STEP 2.5
	boolean desenha = true;
	glBindTexture(GL_TEXTURE_2D, texID);
	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1);
	for (float i = -300; i<300; i += STEP)
		for (float j = -300; j<300; j += STEP){
		if (quantidadeAlcapoes > 0){
			desenha = true;
			for (size_t z = 0; z < quantidadeAlcapoes; z++)
			{
				if (i >= (posxalcapao[z] * 5) - 2.5 && i <= (posxalcapao[z] * 5) && j >= (posyalcapao[z] * 5) - 2.5 && j <= (posyalcapao[z] * 5)){
					desenha = false;
					//DesenhaAlcapao(posxalcapao[z]*5, posyalcapao[z] *5);
					break;
				}
			}
			for (size_t z = 0; z < quantidadeAlcapoesQuartoSecreto; z++)
			{
				if (i >= (posxalcapaoQuartoSecreto[z] * 5) - 2.5 && i <= (posxalcapaoQuartoSecreto[z] * 5) && j >= (posyalcapaoQuartoSecreto[z] * 5) - 2.5 && j <= (posyalcapaoQuartoSecreto[z] * 5)){
					desenha = false;
					break;
				}
			}
			if (desenha){
				glTexCoord2f(1, 1);
				glVertex2f(i, j);
				glTexCoord2f(0, 1);
				glVertex2f(i + STEP, j);
				glTexCoord2f(0, 0);
				glVertex2f(i + STEP, j + STEP);
				glTexCoord2f(1, 0);
				glVertex2f(i, j + STEP);
			}
		}
		else{
			glTexCoord2f(1, 1);
			glVertex2f(i, j);
			glTexCoord2f(0, 1);
			glVertex2f(i + STEP, j);
			glTexCoord2f(0, 0);
			glVertex2f(i + STEP, j + STEP);
			glTexCoord2f(1, 0);
			glVertex2f(i, j + STEP);
		}
		}
	glEnd();
	glBindTexture(GL_TEXTURE_2D, NULL);
}

GLfloat InnerProduct(GLfloat v[], GLfloat q[])
{
	return ((v)[0] * (q)[0] +
		(v)[1] * (q)[1] +
		(v)[2] * (q)[2]);
}

void CrossProduct(GLdouble v1[], GLdouble v2[], GLdouble cross[])
{
	cross[0] = v1[1] * v2[2] - v1[2] * v2[1];
	cross[1] = v1[2] * v2[0] - v1[0] * v2[2];
	cross[2] = v1[0] * v2[1] - v1[1] * v2[0];
}

GLdouble VectorNormalize(GLdouble v[])
{
	int		i;
	GLdouble	length;

	if (fabs(v[1] - 0.000215956) < 0.0001)
		i = 1;

	length = 0;
	for (i = 0; i < 3; i++)
		length += v[i] * v[i];
	length = sqrt(length);
	if (length == 0)
		return 0;

	for (i = 0; i < 3; i++)
		v[i] /= length;

	return length;
}

void desenhaNormal(GLdouble x, GLdouble y, GLdouble z, GLdouble normal[], tipo_material mat){

	switch (mat){
	case red_plastic:
		glColor3f(1, 0, 0);
		break;
	case azul:
		glColor3f(0, 0, 1);
		break;
	case emerald:
		glColor3f(0, 1, 0);
		break;
	default:
		glColor3f(1, 1, 0);
	}
	glDisable(GL_LIGHTING);
	glPushMatrix();
	glTranslated(x, y, z);
	glScaled(0.4, 0.4, 0.4);
	glBegin(GL_LINES);
	glVertex3d(0, 0, 0);
	glVertex3dv(normal);
	glEnd();
	glPopMatrix();
	glEnable(GL_LIGHTING);
}

void desenhaParede(GLfloat xi, GLfloat yi, GLfloat zi, GLfloat xf, GLfloat yf, GLfloat zf, GLuint texID, Estado estado){
	GLdouble v1[3], v2[3], cross[3];
	GLdouble length;
	v1[0] = xf - xi;
	v1[1] = yf - yi;
	v1[2] = 0;
	v2[0] = 0;
	v2[1] = 0;
	v2[2] = 1;
	CrossProduct(v1, v2, cross);
	length = VectorNormalize(cross);
	//material(emerald);
	material(slate);
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
		GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_NEAREST);
	/*glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64,
	64, 0, GL_RGBA, GL_UNSIGNED_BYTE,
	&texID);*/

	///* Código anterior (início) */
	//glBegin(GL_QUADS);
	//glNormal3dv(cross);
	///*glTexCoord2f(tx + 0, ty + 0);*/
	//glTexCoord2f(0, 0);
	//glVertex3f(xi, yi, zi);
	////glTexCoord2f(tx + 0, ty + 0.25);
	//glTexCoord2f(0, 0.25);
	//glVertex3f(xf, yf, zf + 0);
	////glTexCoord2f(tx + 0.25, ty + 0.25);
	//glTexCoord2f(0.25, 0.25);
	//glVertex3f(xf, yf, zf + 1);
	////glTexCoord2f(tx + 0.25, ty + 0);
	//glTexCoord2f(0.25, 0);
	//glVertex3f(xi, yi, zi + 1);
	//glEnd();
	///* Código anterior (fim) */

	/* Código do Nuno (início) */
	if (length == 1){
		glBegin(GL_QUADS);
		glNormal3dv(cross);
		/*glTexCoord2f(tx + 0, ty + 0);*/
		glTexCoord2f(0, 0);
		glVertex3f(xi, yi, zi);
		//glTexCoord2f(tx + 0, ty + 0.25);
		glTexCoord2f(0, 0.25);
		glVertex3f(xf, yf, zf + 0);
		//glTexCoord2f(tx + 0.25, ty + 0.25);
		glTexCoord2f(0.25, 0.25);
		glVertex3f(xf, yf, zf + 1);
		//glTexCoord2f(tx + 0.25, ty + 0);
		glTexCoord2f(0.25, 0);
		glVertex3f(xi, yi, zi + 1);
		glEnd();
	}
	else
	{
		GLfloat xini = xi;
		GLfloat xfin = xf;
		GLfloat yini = yi;
		GLfloat yfin = yf;
		GLfloat zini = zi;
		GLfloat zfin = zf;
		GLfloat interX = xfin - xini;
		GLfloat interY = yfin - yini;
		/*int interX = abs((xini-xfin) / length);
		int interY = abs((yini-yfin) / length);*/
		if (interX == 0 || interY == 0){
			for (int i = 0; i < abs(interX + interY); i++)
			{
				if (interX > 0){
					xfin = xini + 1;
				}
				if (interY > 0){
					yfin = yini + 1;
				}

				if (interX < 0){
					xfin = xini - 1;
				}
				if (interY < 0){
					yfin = yini - 1;
				}
				//yfin = yini + interY;
				glBegin(GL_QUADS);
				glNormal3dv(cross);
				/*glTexCoord2f(tx + 0, ty + 0);*/
				glTexCoord2f(0, 0);
				glVertex3f(xini, yini, zi);
				//glTexCoord2f(tx + 0, ty + 0.25);
				glTexCoord2f(0, 0.25);
				glVertex3f(xfin, yfin, zf + 0);
				/*glVertex3f(xi + i, yi + i, zi + 0);*/
				//glTexCoord2f(tx + 0.25, ty + 0.25);
				glTexCoord2f(0.25, 0.25);
				glVertex3f(xfin, yfin, zf + 1);
				/*glVertex3f(xi + i, yi + i, zi + 1);*/
				//glTexCoord2f(tx + 0.25, ty + 0);
				glTexCoord2f(0.25, 0);
				glVertex3f(xini, yini, zi + 1);
				glEnd();
				xini = xfin;
				yini = yfin;
			}
		}
		else
		{
			GLfloat dist = sqrt(pow(abs(interX), 2) + pow(abs(interY), 2));
			for (GLfloat i = 0; i < dist; i++)
			{

				if (interX > 0){
					xfin = xini + abs(interX) / dist;
					if (xfin > xf)
					{
						xfin = xf;
					}
				}
				if (interY > 0){
					yfin = yini + abs(interY) / dist;
					if (yfin > yf)
					{
						yfin = yf;
					}
				}

				if (interX < 0){
					xfin = xini - abs(interX) / dist;
					if (xfin < xf)
					{
						xfin = xf;
					}
				}
				if (interY < 0){
					yfin = yini - abs(interY) / dist;
					if (yfin < yf)
					{
						yfin = yf;
					}
				}
				//yfin = yini + interY;
				glBegin(GL_QUADS);
				glNormal3dv(cross);
				/*glTexCoord2f(tx + 0, ty + 0);*/
				glTexCoord2f(0, 0);
				glVertex3f(xini, yini, zi);
				//glTexCoord2f(tx + 0, ty + 0.25);
				glTexCoord2f(0, 0.25);
				glVertex3f(xfin, yfin, zf + 0);
				/*glVertex3f(xi + i, yi + i, zi + 0);*/
				//glTexCoord2f(tx + 0.25, ty + 0.25);
				glTexCoord2f(0.25, 0.25);
				glVertex3f(xfin, yfin, zf + 1);
				/*glVertex3f(xi + i, yi + i, zi + 1);*/
				//glTexCoord2f(tx + 0.25, ty + 0);
				glTexCoord2f(0.25, 0);
				glVertex3f(xini, yini, zi + 1);
				glEnd();
				xini = xfin;
				yini = yfin;
			}
		}
	}
	/* Código do Nuno (fim) */

	//glBegin(GL_QUADS);
	//glNormal3dv(cross);
	///*glTexCoord2f(tx + 0, ty + 0);*/
	//glTexCoord2f(0, 0);
	//glVertex3f(xi, yi, zi);
	////glTexCoord2f(tx + 0, ty + 0.25);
	//glTexCoord2f(0, 0.25);
	//glVertex3f(xf, yf, zf + 0);
	////glTexCoord2f(tx + 0.25, ty + 0.25);
	//glTexCoord2f(0.25, 0.25);
	//glVertex3f(xf, yf, zf + 1);
	////glTexCoord2f(tx + 0.25, ty + 0);
	//glTexCoord2f(0.25, 0);
	//glVertex3f(xi, yi, zi + 1);
	//glEnd();

	glBindTexture(GL_TEXTURE_2D, NULL);
	if (estado.apresentaNormais) {
		desenhaNormal(xi, yi, zi, cross, emerald);
		desenhaNormal(xf, yf, zf, cross, emerald);
		desenhaNormal(xf, yf, zf + 1, cross, emerald);
		desenhaNormal(xi, yi, zi + 1, cross, emerald);
	}
}

void desenhaRampa(GLfloat xi, GLfloat yi, GLfloat zi, GLfloat xf, GLfloat yf, GLfloat zf, GLfloat largura, int orient, Estado estado){
	GLdouble v1[3], v2[3], cross[3];
	GLdouble length;
	v1[0] = xf - xi;
	v1[1] = 0;
	v2[0] = 0;
	v2[1] = yf - yi;

	switch (orient) {
	case NORTE_SUL:
		v1[2] = 0;
		v2[2] = zf - zi;
		CrossProduct(v1, v2, cross);
		length = VectorNormalize(cross);

		material(red_plastic);
		glBegin(GL_QUADS);
		glNormal3dv(cross);
		glVertex3f(xi, yi, zi);
		glVertex3f(xf, yi, zi);
		glVertex3f(xf, yf, zf);
		glVertex3f(xi, yf, zf);
		glEnd();
		if (estado.apresentaNormais) {
			desenhaNormal(xi, yi, zi, cross, red_plastic);
			desenhaNormal(xf, yi, zi, cross, red_plastic);
			desenhaNormal(xf, yf, zf, cross, red_plastic);
			desenhaNormal(xi, yi, zf, cross, red_plastic);
		}
		break;
	case ESTE_OESTE:
		v1[2] = zf - zi;
		v2[2] = 0;
		CrossProduct(v1, v2, cross);
		length = VectorNormalize(cross);
		material(red_plastic);
		glBegin(GL_QUADS);
		glNormal3dv(cross);
		glVertex3f(xi, yi, zi);
		glVertex3f(xf, yi, zf);
		glVertex3f(xf, yf, zf);
		glVertex3f(xi, yf, zi);
		glEnd();
		if (estado.apresentaNormais) {
			desenhaNormal(xi, yi, zi, cross, red_plastic);
			desenhaNormal(xf, yi, zf, cross, red_plastic);
			desenhaNormal(xf, yf, zf, cross, red_plastic);
			desenhaNormal(xi, yi, zi, cross, red_plastic);
		}
		break;
	case OBLIQUA:
		//system("pause");
		v1[2] = zf - zi;
		v2[2] = 0;
		CrossProduct(v1, v2, cross);
		length = VectorNormalize(cross);
		material(red_plastic);
		glBegin(GL_QUADS);
		glNormal3dv(cross);
		largura = largura / 2;
		if (yi < yf){
			glVertex3f(xi - largura, yi + largura, zi);
			glVertex3f(xi + largura, yi - largura, zi);
			glVertex3f(xf + largura, yf - largura, zf);
			glVertex3f(xf - largura, yf + largura, zf);
		}
		else{
			glVertex3f(xi - largura, yi - largura, zi);
			glVertex3f(xi + largura, yi + largura, zi);
			glVertex3f(xf + largura, yf + largura, zf);
			glVertex3f(xf - largura, yf - largura, zf);
		}
		glEnd();
		if (estado.apresentaNormais) {
			desenhaNormal(xi, yi, zi, cross, red_plastic);
			desenhaNormal(xf, yi, zf, cross, red_plastic);
			desenhaNormal(xf, yf, zf, cross, red_plastic);
			desenhaNormal(xi, yi, zi, cross, red_plastic);
		}
		break;
	case RAMPA:
		//cout << xi << " " << yi << " " << zi << endl;
		//cout << xf << " " << yf << " " << zf << endl;
		v1[2] = zf - zi;
		v2[2] = 0;
		CrossProduct(v1, v2, cross);
		length = VectorNormalize(cross);
		material(red_plastic);
		glBegin(GL_QUADS);
		glNormal3dv(cross);
		if (yi < yf && xi == xf){
			glVertex3f(xi - 0.5, yi - 0.5, zi);
			glVertex3f(xi + 0.5, yi - 0.5, zi);
			glVertex3f(xf + 0.5, yf - 0.5, zf);
			glVertex3f(xf - 0.5, yf - 0.5, zf);
		}
		else if (yi > yf && xi == xf){
			glVertex3f(xi - 0.5, yi + 0.5, zi);
			glVertex3f(xi + 0.5, yi + 0.5, zi);
			glVertex3f(xf + 0.5, yf + 0.5, zf);
			glVertex3f(xf - 0.5, yf + 0.5, zf);
		}
		else if (yi == yf && xi > xf){
			glVertex3f(xi + 0.5, yi - 0.5, zi);
			glVertex3f(xi + 0.5, yi + 0.5, zi);
			glVertex3f(xf + 0.5, yf + 0.5, zf);
			glVertex3f(xf + 0.5, yf - 0.5, zf);
		}
		else if (yi == yf && xi < xf){
			glVertex3f(xi - 0.5, yi - 0.5, zi);
			glVertex3f(xi - 0.5, yi + 0.5, zi);
			glVertex3f(xf - 0.5, yf + 0.5, zf);
			glVertex3f(xf - 0.5, yf - 0.5, zf);
		}
		glEnd();
		if (estado.apresentaNormais) {
			desenhaNormal(xi - 0.5, yi - 0.5, zi, cross, red_plastic);
			desenhaNormal(xi + 0.5, yi - 0.5, zi, cross, red_plastic);
			desenhaNormal(xf + 0.5, yf - 0.5, zf, cross, red_plastic);
			desenhaNormal(xf - 0.5, yf - 0.5, zf, cross, red_plastic);
		}
		break;
	default:
		cross[0] = 0;
		cross[1] = 0;
		cross[2] = 1;
		material(azul);
		glBegin(GL_QUADS);
		glNormal3f(0, 0, 1);
		glVertex3f(xi, yi, zi);
		glVertex3f(xf, yi, zf);
		glVertex3f(xf, yf, zf);
		glVertex3f(xi, yf, zi);
		glEnd();
		if (estado.apresentaNormais) {
			desenhaNormal(xi, yi, zi, cross, azul);
			desenhaNormal(xf, yi, zf, cross, azul);
			desenhaNormal(xf, yf, zf, cross, azul);
			desenhaNormal(xi, yi, zi, cross, azul);
		}
		break;
	}
}

void desenhaNo(int no, Arco arcos[], No nos[], int numArcos, Modelo modelo, Estado estado){
	GLboolean norte, sul, este, oeste;
	GLfloat larguraNorte, larguraSul, larguraEste, larguraOeste;
	Arco arco = arcos[0];
	No *noi = &nos[no], *nof;
	norte = sul = este = oeste = GL_TRUE;
	//desenhaChao(nos[no].x - 0.5*noi->largura, nos[no].y - 0.5*noi->largura, nos[no].z, nos[no].x + 0.5*noi->largura, nos[no].y + 0.5*noi->largura, nos[no].z, PLANO);
	for (int i = 0; i < numArcos; arco = arcos[++i]){
		if (arco.noi == no)
			nof = &nos[arco.nof];
		else
			if (arco.nof == no)
				nof = &nos[arco.noi];
			else
				continue;
		if (noi->x == nof->x)
			if (noi->y < nof->y){
				norte = GL_FALSE;
				larguraNorte = arco.largura;
			}
			else{
				sul = GL_FALSE;
				larguraSul = arco.largura;
			}
		else
			if (noi->y == nof->y)
				if (noi->x < nof->x){
					oeste = GL_FALSE;
					larguraOeste = arco.largura;
				}
				else{
					este = GL_FALSE;
					larguraEste = arco.largura;
				}
			else
				//	cout << "Arco dioagonal: " << arco.noi << " " << arco.nof << endl;
				if (norte && sul && este && oeste)
					return;
	}
	if (norte)
		desenhaParede(nos[no].x - 0.5*noi->largura, nos[no].y + 0.5*noi->largura, nos[no].z, nos[no].x + 0.5*noi->largura, nos[no].y + 0.5*noi->largura, nos[no].z, modelo.texID[ID_TEXTURA_CUBOS], estado);
	else
		if (larguraNorte < noi->largura){
			desenhaParede(nos[no].x - 0.5*noi->largura, nos[no].y + 0.5*noi->largura, nos[no].z, nos[no].x - 0.5*larguraNorte, nos[no].y + 0.5*noi->largura, nos[no].z, modelo.texID[ID_TEXTURA_CUBOS], estado);
			desenhaParede(nos[no].x + 0.5*larguraNorte, nos[no].y + 0.5*noi->largura, nos[no].z, nos[no].x + 0.5*noi->largura, nos[no].y + 0.5*noi->largura, nos[no].z, modelo.texID[ID_TEXTURA_CUBOS], estado);
		}
	if (sul)
		desenhaParede(nos[no].x + 0.5*noi->largura, nos[no].y - 0.5*noi->largura, nos[no].z, nos[no].x - 0.5*noi->largura, nos[no].y - 0.5*noi->largura, nos[no].z, modelo.texID[ID_TEXTURA_CUBOS], estado);
	else
		if (larguraSul < noi->largura){
			desenhaParede(nos[no].x + 0.5*noi->largura, nos[no].y - 0.5*noi->largura, nos[no].z, nos[no].x + 0.5*larguraSul, nos[no].y - 0.5*noi->largura, nos[no].z, modelo.texID[ID_TEXTURA_CUBOS], estado);
			desenhaParede(nos[no].x - 0.5*larguraSul, nos[no].y - 0.5*noi->largura, nos[no].z, nos[no].x - 0.5*noi->largura, nos[no].y - 0.5*noi->largura, nos[no].z, modelo.texID[ID_TEXTURA_CUBOS], estado);
		}
	if (este)
		desenhaParede(nos[no].x - 0.5*noi->largura, nos[no].y - 0.5*noi->largura, nos[no].z, nos[no].x - 0.5*noi->largura, nos[no].y + 0.5*noi->largura, nos[no].z, modelo.texID[ID_TEXTURA_CUBOS], estado);
	else
		if (larguraEste < noi->largura){
			desenhaParede(nos[no].x - 0.5*noi->largura, nos[no].y - 0.5*noi->largura, nos[no].z, nos[no].x - 0.5*noi->largura, nos[no].y - 0.5*larguraEste, nos[no].z, modelo.texID[ID_TEXTURA_CUBOS], estado);
			desenhaParede(nos[no].x - 0.5*noi->largura, nos[no].y + 0.5*larguraEste, nos[no].z, nos[no].x - 0.5*noi->largura, nos[no].y + 0.5*noi->largura, nos[no].z, modelo.texID[ID_TEXTURA_CUBOS], estado);
		}
	if (oeste)
		desenhaParede(nos[no].x + 0.5*noi->largura, nos[no].y + 0.5*noi->largura, nos[no].z, nos[no].x + 0.5*noi->largura, nos[no].y - 0.5*noi->largura, nos[no].z, modelo.texID[ID_TEXTURA_CUBOS], estado);
	else
		if (larguraOeste < noi->largura){
			desenhaParede(nos[no].x + 0.5*noi->largura, nos[no].y + 0.5*noi->largura, nos[no].z, nos[no].x + 0.5*noi->largura, nos[no].y + 0.5*larguraOeste, nos[no].z, modelo.texID[ID_TEXTURA_CUBOS], estado);
			desenhaParede(nos[no].x + 0.5*noi->largura, nos[no].y - 0.5*larguraOeste, nos[no].z, nos[no].x + 0.5*noi->largura, nos[no].y - 0.5*noi->largura, nos[no].z, modelo.texID[ID_TEXTURA_CUBOS], estado);
		}
}

void desenhaArco(Arco arco, No nos[], Modelo modelo, Estado estado){
	No *noi, *nof;
		if (nos[arco.noi].x == nos[arco.nof].x){
			// arco vertical
			if (nos[arco.noi].y < nos[arco.nof].y){
				noi = &nos[arco.noi];
				nof = &nos[arco.nof];
			}
			else{
				nof = &nos[arco.noi];
				noi = &nos[arco.nof];
			}
			if (flag <= 1){
				paredes[numParedes].xi = (noi->x - 0.5*arco.largura) * 5 - 0.7;
				paredes[numParedes].yi = (noi->y - 0.5*arco.largura) * 5 - 0.7;
				paredes[numParedes].xf = (nof->x + 0.5*arco.largura) * 5 + 0.7;
				paredes[numParedes].yf = (nof->y + 0.5*arco.largura) * 5 + 0.7;
				paredes[numParedes].tipo = "parede_reta";
				numParedes++;
			}
			desenhaRampa(noi->x - 0.5*arco.largura, noi->y - 0.5*noi->largura, noi->z + 1, nof->x + 0.5*arco.largura, nof->y + 0.5*nof->largura, nof->z + 1, 0, NORTE_SUL, estado);
			desenhaParede(noi->x - 0.5*arco.largura, noi->y + 0.5*noi->largura, noi->z, nof->x - 0.5*arco.largura, nof->y - 0.5*nof->largura, nof->z, modelo.texID[ID_TEXTURA_CUBOS], estado);
			desenhaParede(nof->x + 0.5*arco.largura, nof->y - 0.5*nof->largura, nof->z, noi->x + 0.5*arco.largura, noi->y + 0.5*noi->largura, noi->z, modelo.texID[ID_TEXTURA_CUBOS], estado);
		}
		else{
			if (nos[arco.noi].y == nos[arco.nof].y){
				//arco horizontal
				if (nos[arco.noi].x < nos[arco.nof].x){
					noi = &nos[arco.noi];
					nof = &nos[arco.nof];
				}
				else{
					nof = &nos[arco.noi];
					noi = &nos[arco.nof];
				}
				if (flag <= 1){
					paredes[numParedes].xi = (noi->x - 0.5*arco.largura) * 5 - 0.7;
					paredes[numParedes].yi = (noi->y - 0.5*arco.largura) * 5 - 0.7;
					paredes[numParedes].xf = (nof->x + 0.5*arco.largura) * 5 + 0.7;
					paredes[numParedes].yf = (nof->y + 0.5*arco.largura) * 5 + 0.7;
					paredes[numParedes].tipo = "parede_reta";
					numParedes++;
				}
				desenhaRampa(noi->x - 0.5*noi->largura, noi->y - 0.5*arco.largura, noi->z + 1, nof->x + 0.5*nof->largura, nof->y + 0.5*arco.largura, nof->z + 1, 0, ESTE_OESTE, estado);
				desenhaParede(noi->x - 0.5*noi->largura, noi->y + 0.5*arco.largura, noi->z, nof->x - 0.5*nof->largura, nof->y + 0.5*arco.largura, nof->z, modelo.texID[ID_TEXTURA_CUBOS], estado);
				desenhaParede(nof->x - 0.5*noi->largura, nof->y - 0.5*arco.largura, nof->z, noi->x - 0.5*noi->largura, noi->y - 0.5*arco.largura, noi->z, modelo.texID[ID_TEXTURA_CUBOS], estado);
			}
			else{
				if (nos[arco.noi].y < nos[arco.nof].y){
					//diagonal a subir
					noi = &nos[arco.noi];
					nof = &nos[arco.nof];

					pos_t P1, P2, P3, P4;
					P1.x = noi->x + noi->largura / 2;
					P1.y = noi->y - noi->largura / 2;
					P1.z = noi->z;
					P2.x = noi->x - noi->largura / 2;
					P2.y = noi->y + noi->largura / 2;
					P2.z = noi->z;

					P3.x = nof->x + noi->largura / 2;
					P3.y = nof->y - noi->largura / 2;
					P3.z = nof->z;
					P4.x = nof->x - noi->largura / 2;
					P4.y = nof->y + noi->largura / 2;
					P4.z = nof->z;

					glPushMatrix();
					desenhaParede(P1.x, P1.y, P1.z, P3.x, P3.y, P3.z, modelo.texID[ID_TEXTURA_CUBOS], estado);
					desenhaParede(P2.x, P2.y, P2.z, P4.x, P4.y, P4.z, modelo.texID[ID_TEXTURA_CUBOS], estado);


					desenhaParede(P1.x, P1.y, P1.z, P2.x, P2.y, P2.z, modelo.texID[ID_TEXTURA_CUBOS], estado);
					desenhaParede(P3.x, P3.y, P3.z, P4.x, P4.y, P4.z, modelo.texID[ID_TEXTURA_CUBOS], estado);
					desenhaRampa(noi->x, noi->y, noi->z + 1, nof->x, nof->y, nof->z + 1, noi->largura, OBLIQUA, estado);
					glPopMatrix();

					if (flag <= 1){
						paredes[numParedes].xi = P1.x * 5 + noi->largura / 2;
						paredes[numParedes].yi = P1.y * 5 - noi->largura / 2;
						paredes[numParedes].xf = P3.x * 5 + noi->largura / 2;
						paredes[numParedes].yf = P3.y * 5 - noi->largura / 2;

						paredes[numParedes].xi2 = P2.x * 5 - noi->largura / 2;
						paredes[numParedes].yi2 = P2.y * 5 + noi->largura / 2;
						paredes[numParedes].xf2 = P4.x * 5 - noi->largura / 2;
						paredes[numParedes].yf2 = P4.y * 5 + noi->largura / 2;

						paredes[numParedes].tipo = "parede_obliqua";
						numParedes++;
					}

				}
				else{
					//diagonal a descer
					noi = &nos[arco.noi];
					nof = &nos[arco.nof];

					pos_t P1, P2, P3, P4;
					P1.x = noi->x - noi->largura / 2;
					P1.y = noi->y - noi->largura / 2;
					P1.z = noi->z;
					P2.x = noi->x + noi->largura / 2;
					P2.y = noi->y + noi->largura / 2;
					P2.z = noi->z;

					P3.x = nof->x - noi->largura / 2;
					P3.y = nof->y - noi->largura / 2;
					P3.z = nof->z;
					P4.x = nof->x + noi->largura / 2;
					P4.y = nof->y + noi->largura / 2;
					P4.z = nof->z;

					glPushMatrix();
					desenhaParede(P1.x, P1.y, P1.z, P3.x, P3.y, P3.z, modelo.texID[ID_TEXTURA_CUBOS], estado);
					desenhaParede(P2.x, P2.y, P2.z, P4.x, P4.y, P4.z, modelo.texID[ID_TEXTURA_CUBOS], estado);


					desenhaParede(P1.x, P1.y, P1.z, P2.x, P2.y, P2.z, modelo.texID[ID_TEXTURA_CUBOS], estado);
					desenhaParede(P3.x, P3.y, P3.z, P4.x, P4.y, P4.z, modelo.texID[ID_TEXTURA_CUBOS], estado);
					desenhaRampa(noi->x, noi->y, noi->z + 1, nof->x, nof->y, nof->z + 1,noi->largura, OBLIQUA, estado);
					glPopMatrix();

					if (flag <= 1){
						paredes[numParedes].xi = P1.x * 5 - noi->largura / 2;
						paredes[numParedes].yi = P1.y * 5 - noi->largura / 2;
						paredes[numParedes].xf = P3.x * 5 - noi->largura / 2;
						paredes[numParedes].yf = P3.y * 5 - noi->largura / 2;

						paredes[numParedes].xi2 = P2.x * 5 + noi->largura / 2;
						paredes[numParedes].yi2 = P2.y * 5 + noi->largura / 2;
						paredes[numParedes].xf2 = P4.x * 5 + noi->largura / 2;
						paredes[numParedes].yf2 = P4.y * 5 + noi->largura / 2;

						paredes[numParedes].tipo = "parede_obliqua";
						numParedes++;
					}
				}
			}
		}
	
}

void desenhaRampas(No rampa, No nos[], int numNos, Modelo modelo, Estado estado){
	int i;
	for (i = 0; i < numNos; i++){
		if (nos[i].x == rampa.x && rampa.y + 1 == nos[i].y){
			//rampa para cima
			if (flag <= 1){
				paredes[numParedes].xi = rampa.x * 5;
				paredes[numParedes].yi = rampa.y * 5;
				paredes[numParedes].xf = nos[i].x * 5;
				paredes[numParedes].yf = nos[i].y * 5;
				paredes[numParedes].tipo = "rampaCima";
				numParedes++;

				//colisão parede1
				paredes[numParedes].xi = (rampa.x + 0.5) * 5;
				paredes[numParedes].yi = (rampa.y - 0.5) * 5;
				paredes[numParedes].xf = (nos[i].x + 0.5) * 5 + 0.5;
				paredes[numParedes].yf = (nos[i].y - 0.5) * 5;
				paredes[numParedes].tipo = "parede_reta";
				numParedes++;
				//colisão parede2
				paredes[numParedes].xi = (rampa.x - 0.5) * 5;
				paredes[numParedes].yi = (rampa.y - 0.5) * 5;
				paredes[numParedes].xf = (nos[i].x - 0.5) * 5 - 0.5;
				paredes[numParedes].yf = (nos[i].y - 0.5) * 5;
				paredes[numParedes].tipo = "parede_reta";
				numParedes++;
			}
			desenhaRampa(rampa.x, rampa.y, rampa.z, nos[i].x, nos[i].y, nos[i].z + 1, 0, RAMPA, estado);
			desenhaParede(rampa.x + 0.5, rampa.y - 0.5, rampa.z - 0.5, nos[i].x + 0.5, nos[i].y - 0.5, nos[i].z+0.5, modelo.texID[ID_TEXTURA_CUBOS], estado);
			desenhaParede(rampa.x - 0.5, rampa.y - 0.5, rampa.z - 0.5, nos[i].x - 0.5, nos[i].y - 0.5, nos[i].z + 0.5, modelo.texID[ID_TEXTURA_CUBOS], estado);
		}
		else if (nos[i].x == rampa.x && rampa.y - 1 == nos[i].y){
			//rampa para baixo
			//cout << "baixo" << endl;
			if (flag <= 1){
				//colisão rampa
				paredes[numParedes].xi = (rampa.x) * 5;
				paredes[numParedes].yi = (rampa.y) * 5;
				paredes[numParedes].xf = (nos[i].x) * 5;
				paredes[numParedes].yf = (nos[i].y) * 5;
				paredes[numParedes].tipo = "rampaBaixo";
				numParedes++;

				//colisão parede1
				paredes[numParedes].xi = (rampa.x + 0.5) * 5;
				paredes[numParedes].yi = (rampa.y + 0.5) * 5;
				paredes[numParedes].xf = (nos[i].x + 0.5) * 5 + 0.5;
				paredes[numParedes].yf = (nos[i].y + 0.5) * 5;
				paredes[numParedes].tipo = "parede_reta";
				numParedes++;
				//colisão parede2
				paredes[numParedes].xi = (rampa.x - 0.5) * 5;
				paredes[numParedes].yi = (rampa.y + 0.5) * 5;
				paredes[numParedes].xf = (nos[i].x - 0.5) * 5 - 0.5;
				paredes[numParedes].yf = (nos[i].y + 0.5) * 5;
				paredes[numParedes].tipo = "parede_reta";
				numParedes++;
			}
			desenhaRampa(rampa.x, rampa.y, rampa.z, nos[i].x, nos[i].y, nos[i].z + 1, 0, RAMPA, estado);
			desenhaParede(rampa.x + 0.5, rampa.y + 0.5, rampa.z - 0.5, nos[i].x + 0.5, nos[i].y + 0.5, nos[i].z + 0.5, modelo.texID[ID_TEXTURA_CUBOS], estado);
			desenhaParede(rampa.x - 0.5, rampa.y + 0.5, rampa.z - 0.5, nos[i].x - 0.5, nos[i].y + 0.5, nos[i].z + 0.5, modelo.texID[ID_TEXTURA_CUBOS], estado);
		}
		else if (nos[i].y == rampa.y && rampa.x + 1 == nos[i].x){
			//rampa para direita
			if (flag <= 1){
				paredes[numParedes].xi = rampa.x * 5;
				paredes[numParedes].yi = rampa.y * 5;
				paredes[numParedes].xf = nos[i].x * 5;
				paredes[numParedes].yf = nos[i].y * 5;
				paredes[numParedes].tipo = "rampaDireita";
				numParedes++;

				//colisão parede1
				paredes[numParedes].xi = (rampa.x - 0.5) * 5;
				paredes[numParedes].yi = (rampa.y + 0.5) * 5;
				paredes[numParedes].xf = (nos[i].x - 0.5) * 5 ;
				paredes[numParedes].yf = (nos[i].y + 0.5) * 5 +0.5;
				paredes[numParedes].tipo = "parede_reta";
				numParedes++;
				//colisão parede2
				paredes[numParedes].xi = (rampa.x - 0.5) * 5;
				paredes[numParedes].yi = (rampa.y - 0.5) * 5;
				paredes[numParedes].xf = (nos[i].x - 0.5) * 5 ;
				paredes[numParedes].yf = (nos[i].y - 0.5) * 5-0.5;
				paredes[numParedes].tipo = "parede_reta";
				numParedes++;
			}
			desenhaRampa(rampa.x, rampa.y, rampa.z, nos[i].x, nos[i].y, nos[i].z + 1, 0, RAMPA, estado);
			desenhaParede(rampa.x - 0.5, rampa.y + 0.5, rampa.z - 0.5, nos[i].x - 0.5, nos[i].y + 0.5, nos[i].z+0.5, modelo.texID[ID_TEXTURA_CUBOS], estado);
			desenhaParede(rampa.x - 0.5, rampa.y - 0.5, rampa.z - 0.5, nos[i].x - 0.5, nos[i].y - 0.5, nos[i].z + 0.5, modelo.texID[ID_TEXTURA_CUBOS], estado);
		}
		else if (nos[i].y == rampa.y && rampa.x - 1 == nos[i].x){
			//rampa para esquerda
			if (flag <= 1){
				paredes[numParedes].xi = rampa.x * 5;
				paredes[numParedes].yi = rampa.y * 5;
				paredes[numParedes].xf = nos[i].x * 5;
				paredes[numParedes].yf = nos[i].y * 5;
				paredes[numParedes].tipo = "rampaEsquerda";
				numParedes++;

				//colisão parede1
				paredes[numParedes].xi = (rampa.x + 0.5) * 5;
				paredes[numParedes].yi = (rampa.y + 0.5) * 5;
				paredes[numParedes].xf = (nos[i].x + 0.5) * 5 ;
				paredes[numParedes].yf = (nos[i].y + 0.5) * 5 + 0.5;
				paredes[numParedes].tipo = "parede_reta";
				numParedes++;
				//colisão parede2
				paredes[numParedes].xi = (rampa.x + 0.5) * 5;
				paredes[numParedes].yi = (rampa.y - 0.5) * 5;
				paredes[numParedes].xf = (nos[i].x + 0.5) * 5 ;
				paredes[numParedes].yf = (nos[i].y - 0.5) * 5 - 0.5;
				paredes[numParedes].tipo = "parede_reta";
				numParedes++;
			}
			desenhaRampa(rampa.x, rampa.y, rampa.z, nos[i].x, nos[i].y, nos[i].z + 1, 0, RAMPA, estado);
			desenhaParede(rampa.x + 0.5, rampa.y + 0.5, rampa.z - 0.5, nos[i].x + 0.5, nos[i].y + 0.5, nos[i].z + 0.5, modelo.texID[ID_TEXTURA_CUBOS], estado);
			desenhaParede(rampa.x + 0.5, rampa.y - 0.5, rampa.z - 0.5, nos[i].x + 0.5, nos[i].y - 0.5, nos[i].z + 0.5, modelo.texID[ID_TEXTURA_CUBOS], estado);
		}
	}


}

void desenhaFimMundo(GLdouble nx, GLdouble ny){
	//mdlviewer_init("door.mdl", portas.door);
	glPushMatrix();

	glTranslatef(nx, ny, 0);

	//glRotatef(graus(modelo.objecto.dir), 0, 0, 1);

	//desenhaLanterna();
	//glRotatef(graus, 0, 0, 1);
	glScalef(0.005 * 1.6, 0.005 * 1.6, 0.005 * 1.6);
	mdlviewer_display(fimMundo.modelo);

	glPopMatrix();
}

void desenhaPorta(GLdouble nx, GLdouble ny, GLdouble graus){
	//mdlviewer_init("door.mdl", portas.door);
	glPushMatrix();

	glTranslatef(nx, ny, 0);

	//glRotatef(graus(modelo.objecto.dir), 0, 0, 1);

	//desenhaLanterna();
	glRotatef(graus, 0, 0, 1);
	glScalef(0.005 * 1.6, 0.005 * 1.6, 0.005 * 1.6);
	mdlviewer_display(portas.door);

	glPopMatrix();
}

void desenhaLabirinto(int numNos, No nos[], int numArcos, Arco arcos[], int numRampas, No rampas[], Modelo modelo, Estado estado){
	//mdlviewer_init("door.mdl", portas.door);
	glPushMatrix();
	glTranslatef(0, 0, 0.05);
	glScalef(5, 5, 5);
	No primeiro = nos[0];
	No ultimo = nos[2];
	material(red_plastic);
	for (int i = 0; i < numNos; i++){


		glPushMatrix();
		material(preto);
		glTranslatef(nos[i].x, nos[i].y, nos[i].z + 0.25);
		//glutSolidCube(0.5);

		glPopMatrix();
		desenhaNo(i, arcos, nos, numArcos, modelo, estado);

	}
	material(emerald);
	for (int i = 0; i < numArcos; i++){
		desenhaArco(arcos[i], nos, modelo, estado);
	}
	for (int i = 0; i < numRampas; i++){
		desenhaRampas(rampas[i], nos, numNos, modelo, estado);
	}
	for (int i = 0; i < numPosicaoTrans; i++)
	{
		desenhaPorta(posxtrans[i], posytrans[i], porta[i]);
	}
	desenhaFimMundo(posxfinal[0], posyfinal[0]);
	glPopMatrix();
	flag++;
}

void desenhaBarril(GLdouble nx, GLdouble ny, int i) {
	material(slate);
	glScalef(0.06, 0.06, 0.06);
	mdlviewer_display(barris.barel);

	obstaculos[i].xi = nx - 0.3;
	obstaculos[i].xf = nx + 2.7;
	obstaculos[i].yi = ny - 1.5;
	obstaculos[i].yf = ny + 1.5;
}

void desenhaObstaculos(Menu menu, int quantidadeObstaculos, int posXObstaculos[], int posYObstaculos[])
{
	//mdlviewer_init("barel.mdl", barris.barel);
	for (int i = 0; i < quantidadeObstaculos; i++) {
		glPushMatrix();
		glTranslatef(posXObstaculos[i], posYObstaculos[i], 1.8);
		//desenhaCubo(2.5, posXObstaculos[i], posYObstaculos[i], i);
		desenhaBarril(posXObstaculos[i], posYObstaculos[i], i);
		glPopMatrix();
	}
}