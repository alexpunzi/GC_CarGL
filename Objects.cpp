/****************************************************************************************

    CarGL (Objects.cpp)

    Grado en Ingenieria Multimedia.
    Practica de Graficos por Computador.
    OpenGL con Shaders.
  ---------------------------------------------------------------------------------------

    2012 - 2022 (C) Juan Antonio Puchol García (puchol@dccia.ua.es)


*****************************************************************************************/

#include "Objects.h"
#include <GL/glui.h>

#include "load3ds.c"

// Variable para inicializar los vectores correpondientes con los valores iniciales
GLfloat light0_ambient_c[4]  = {   0.2f,   0.2f,  0.2f, 1.0f };
GLfloat light0_diffuse_c[4]  = {   0.8f,   0.8f,  0.8f, 1.0f };
GLfloat light0_specular_c[4] = {   1.0f,   1.0f,  1.0f, 1.0f };
GLfloat light0_position_c[4] = {-100.0f, 100.0f, 50.0f, 1.0f };

GLfloat light1_ambient_c[4]  = {   0.2f,   0.2f,  0.2f, 1.0f };
GLfloat light1_diffuse_c[4]  = {   0.8f,   0.8f,  0.8f, 1.0f };
GLfloat light1_specular_c[4] = {   1.0f,   1.0f,  1.0f, 1.0f };
GLfloat light1_position_c[4] = {   0.0f, 100.0f,  0.0f, 1.0f };

GLfloat mat_ambient_c[4]    = { 0.7f, 0.7f, 0.7f, 1.0f };
GLfloat mat_diffuse_c[4]    = { 0.8f, 0.8f, 0.8f, 1.0f };
GLfloat mat_specular_c[4]   = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat mat_shininess_c[1] = { 100.0f };

// Matriz de 4x4 = (I)
float view_rotate_c[16] = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
float view_position_c[3] = { 0.0, -2.0, -9.0 };

float coloresc_c[2][4] = { {0.8f, 0.5f, 0.0f, 1.0f}, {0.15f, 0.15f, 0.15f, 1.0f}};	// Color del coche
float coloresr_c[2][4] = { {0.20f, 0.20f, 0.22f, 1.0f}, {1.00f, 0.85f, 0.10f, 1.0f}};	// Color de la carretera

//************************************************************** Variables de clase

TEscena escena;
TGui    gui;

//************************************************************** Clase TPrimitiva

TPrimitiva::TPrimitiva(int DL, int t)
{

    ID   = DL;
    tipo = t;

    sx = sy = sz = 1;
    rx = ry = rz = 0;

    // Valores seguros por si algun .3ds no se encuentra (Load3DS devuelve NULL)
    modelo0 = modelo1 = NULL;
    num_vertices0 = num_vertices1 = 0;

	switch (tipo) {
		case CARRETERA_ID: {	// Carretera (Asfalto gris oscuro + Líneas amarillas)
		    tx = ty = tz = 0;
            colores[0][0] = 0.20f; colores[0][1] = 0.20f; colores[0][2] = 0.22f; colores[0][3] = 1.0f; // Asfalto
            colores[1][0] = 1.00f; colores[1][1] = 0.85f; colores[1][2] = 0.10f; colores[1][3] = 1.0f; // Líneas amarillas

            modelo0 = Load3DS("../../Modelos/Road.3ds", &num_vertices0);
            modelo1 = Load3DS("../../Modelos/Lines.3ds", &num_vertices1);
            break;
		}
		case EDIFICIO_ID: {	// Edificio (Color base Rojo)
		    tx = 15.0; ty = 0.0; tz = 15.0; rr = 0.0;
            colores[0][0] = 0.90f; colores[0][1] = 0.20f; colores[0][2] = 0.20f; colores[0][3] = 1.0f;
            modelo0 = Load3DS("../../Modelos/edificio.3ds", &num_vertices0);
            break;
		}
		case COCHE_ID: {	// Coche (Carrocería Roja + Ruedas Negras)
		    tx = -2.0; ty = 0.93; tz = 0.0; rr = 0.0;
            colores[0][0] = 0.90f; colores[0][1] = 0.15f; colores[0][2] = 0.15f; colores[0][3] = 1.0f; // Carrocería
            colores[1][0] = 0.15f; colores[1][1] = 0.15f; colores[1][2] = 0.15f; colores[1][3] = 1.0f; // Ruedas
            modelo0 = Load3DS("../../Modelos/Carroceria.3ds", &num_vertices0);
            modelo1 = Load3DS("../../Modelos/Rueda.3ds", &num_vertices1);
            break;
		}
		case CRUCE_ID: {	// Cruce (Asfalto cruce central)
		    tx = ty = tz = 0;
            colores[0][0] = 0.20f; colores[0][1] = 0.20f; colores[0][2] = 0.22f; colores[0][3] = 1.0f;
            modelo0 = Load3DS("../../Modelos/Road.3ds", &num_vertices0);
            break;
		}
		case FAROLA_ID: {	// Farola (Amarillo brillante)
            tx = ty = tz = ry = 0.0;
            colores[0][0] = 0.95f; colores[0][1] = 0.85f; colores[0][2] = 0.10f; colores[0][3] = 1.0f;
            modelo0 = Load3DS("../../Modelos/Farola.3ds", &num_vertices0);
            break;
		}
		case FUENTE_ID: {	// Fuente (Azul agua)
            tx = -8.0; ty = 0.0; tz = 10.0; ry = 0.0;
            colores[0][0] = 0.20f; colores[0][1] = 0.60f; colores[0][2] = 0.90f; colores[0][3] = 1.0f;
            modelo0 = Load3DS("../../Modelos/Fuente.3ds", &num_vertices0);
            break;
		}
		case PISTA_ID: {	// Pista deportiva (Superficie roja terracota)
            tx = 25.0; ty = 0.0; tz = -20.0; ry = 0.0;
            colores[0][0] = 0.85f; colores[0][1] = 0.35f; colores[0][2] = 0.15f; colores[0][3] = 1.0f;
            modelo0 = Load3DS("../../Modelos/Pista.3ds", &num_vertices0);
            break;
        }
        case PAPELERA_ID: {	// Papelera (Negra)
            tx = ty = tz = ry = 0.0;
            colores[0][0] = 0.12f; colores[0][1] = 0.12f; colores[0][2] = 0.12f; colores[0][3] = 1.0f;
            modelo0 = Load3DS("../../Modelos/Papelera.3ds", &num_vertices0);
            break;
        }
        case BANCO_ID: {	// Banco de parque (Marrón madera)
            tx = ty = tz = ry = 0.0;
            colores[0][0] = 0.55f; colores[0][1] = 0.27f; colores[0][2] = 0.07f; colores[0][3] = 1.0f;
            modelo0 = Load3DS("../../Modelos/Banco.3ds", &num_vertices0);
            break;
        }
        case SENAL_ID: {	// Señal de tráfico (Rojo tráfico)
            tx = ty = tz = ry = 0.0;
            colores[0][0] = 0.85f; colores[0][1] = 0.10f; colores[0][2] = 0.10f; colores[0][3] = 1.0f;
            modelo0 = Load3DS("../../Modelos/Senal.3ds", &num_vertices0);
            break;
        }
        case SEMAFORO_ID: {	// Semáforo (Amarillo metálico)
            tx = ty = tz = ry = 0.0;
            colores[0][0] = 0.90f; colores[0][1] = 0.70f; colores[0][2] = 0.00f; colores[0][3] = 1.0f;
            modelo0 = Load3DS("../../Modelos/Semaforo.3ds", &num_vertices0);
            break;
        }
        case ARBOL_ID: {	// Árbol (Verde copa)
            tx = ty = tz = ry = 0.0;
            colores[0][0] = 0.15f; colores[0][1] = 0.70f; colores[0][2] = 0.15f; colores[0][3] = 1.0f;
            modelo0 = Load3DS("../../Modelos/Arbol.3ds", &num_vertices0);
            break;
        }
        case PARADA_ID: {	// Parada de autobús (Azul marquesina)
            tx = ty = tz = ry = 0.0;
            colores[0][0] = 0.10f; colores[0][1] = 0.35f; colores[0][2] = 0.75f; colores[0][3] = 1.0f;
            modelo0 = Load3DS("../../Modelos/Parada.3ds", &num_vertices0);
            break;
        }
        case QUISCO_ID: {	// Kiosco (Marrón clarito suave)
            tx = ty = tz = ry = 0.0;
            colores[0][0] = 0.82f; colores[0][1] = 0.68f; colores[0][2] = 0.52f; colores[0][3] = 1.0f;
            modelo0 = Load3DS("../../Modelos/Kiosco.3ds", &num_vertices0);
            if (modelo0 == NULL) {
                modelo0 = Load3DS("../../Modelos/Quisco.3ds", &num_vertices0);
            }
            break;
        }
	} // Switch
}

void __fastcall TPrimitiva::Render(int seleccion, bool reflejo)
{
    glm::mat4   modelMatrix;
    glm::mat4   modelViewMatrix;

    switch (tipo) {
        case CARRETERA_ID: {
            if (escena.show_road) {
                modelMatrix     = glm::mat4(1.0f);
                modelMatrix     = glm::translate(modelMatrix, glm::vec3(tx, ty, tz));
                modelMatrix     = glm::rotate(modelMatrix, (float) glm::radians(ry), glm::vec3(0,1,0));

                modelViewMatrix = escena.viewMatrix * modelMatrix;

                glUniformMatrix4fv(escena.uMVMatrixLocation, 1, GL_FALSE, &modelViewMatrix[0][0]);

                // Pintar la carretera (asfalto)
                glUniform4fv(escena.uColorLocation, 1, colores[0]);
                glVertexAttribPointer(escena.aPositionLocation, POSITION_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo0);
                glVertexAttribPointer(escena.aNormalLocation, NORMAL_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo0+3);
                glDrawArrays(GL_TRIANGLES, 0, num_vertices0);

                // Pintar las lineas
                glUniform4fv(escena.uColorLocation, 1, colores[1]);
                glVertexAttribPointer(escena.aPositionLocation, POSITION_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo1);
                glVertexAttribPointer(escena.aNormalLocation, NORMAL_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo1+3);
                glDrawArrays(GL_TRIANGLES, 0, num_vertices1);
            }
            break;
        }

        case EDIFICIO_ID: {
            if (escena.show_road) {
                modelMatrix     = glm::mat4(1.0f);
                modelMatrix     = glm::translate(modelMatrix, glm::vec3(tx, ty, tz));
                modelMatrix     = glm::rotate(modelMatrix, (float) glm::radians(ry), glm::vec3(0,1,0));
                modelMatrix     = glm::scale(modelMatrix, glm::vec3(sx, sy, sz));

                modelViewMatrix = escena.viewMatrix * modelMatrix;
                glUniformMatrix4fv(escena.uMVMatrixLocation, 1, GL_FALSE, &modelViewMatrix[0][0]);

                // Pintar el edificio
                glUniform4fv(escena.uColorLocation, 1, colores[0]);
                glVertexAttribPointer(escena.aPositionLocation, POSITION_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo0);
                glVertexAttribPointer(escena.aNormalLocation, NORMAL_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo0+3);
                glDrawArrays(GL_TRIANGLES, 0, num_vertices0);
            }
            break;
        }

        case COCHE_ID: {
            glm::mat4 baseMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(tx, ty, tz));
            baseMatrix = glm::rotate(baseMatrix, glm::radians(180.0f + ry), glm::vec3(0.0f, 1.0f, 0.0f));

            if (escena.show_car) {
                glUniform4fv(escena.uColorLocation, 1, (const GLfloat *) colores[0]);
                glVertexAttribPointer(escena.aPositionLocation, POSITION_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo0);
                glVertexAttribPointer(escena.aNormalLocation, NORMAL_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo0+3);

                glm::mat4 modelViewMatrix = escena.viewMatrix * baseMatrix;
                glUniformMatrix4fv(escena.uMVMatrixLocation, 1, GL_FALSE, &modelViewMatrix[0][0]);
                glDrawArrays(GL_TRIANGLES, 0, num_vertices0);
            }

            if (escena.show_wheels) {
                glUniform4fv(escena.uColorLocation, 1, (const GLfloat *) colores[1]);
                glVertexAttribPointer(escena.aPositionLocation, POSITION_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo1);
                glVertexAttribPointer(escena.aNormalLocation, NORMAL_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo1+3);

                static const float rueda[4][3] = {
                    {-0.864f, -0.5f, -1.294f},
                    { 0.864f, -0.5f, -1.294f},
                    {-0.864f, -0.5f,  1.428f},
                    { 0.864f, -0.5f,  1.428f}
                };

                for (int i = 0; i < 4; i++) {
                    glm::mat4 modelMatrix = glm::translate(baseMatrix, glm::vec3(rueda[i][0], rueda[i][1], rueda[i][2]));
                    modelMatrix = glm::rotate(modelMatrix, (float) glm::radians(rr), glm::vec3(1.0f, 0.0f, 0.0f));

                    if (rueda[i][0] < 0)
                        modelMatrix = glm::rotate(modelMatrix, (float) glm::radians(180.0), glm::vec3(0.0f, 0.0f, 1.0f));

                    glm::mat4 modelViewMatrix = escena.viewMatrix * modelMatrix;
                    glUniformMatrix4fv(escena.uMVMatrixLocation, 1, GL_FALSE, &modelViewMatrix[0][0]);
                    glDrawArrays(GL_TRIANGLES, 0, num_vertices1);
                }
            }
            break;
        }

        case CRUCE_ID:
        case FAROLA_ID:
        case FUENTE_ID:
        case PISTA_ID:
        case PAPELERA_ID:
        case BANCO_ID:
        case SENAL_ID:
        case SEMAFORO_ID:
        case ARBOL_ID:
        case PARADA_ID:
        case QUISCO_ID: {
            if (modelo0 == NULL) break;
            if (tipo == CRUCE_ID && !escena.show_road) break;

            glUniform4fv(escena.uColorLocation, 1, (const GLfloat *) colores[0]);
            glVertexAttribPointer(escena.aPositionLocation, POSITION_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo0);
            glVertexAttribPointer(escena.aNormalLocation, NORMAL_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo0+3);

            modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(tx, ty, tz));
            modelMatrix = glm::rotate(modelMatrix, glm::radians(ry), glm::vec3(0.0f, 1.0f, 0.0f));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(sx, sy, sz));

            modelViewMatrix = escena.viewMatrix * modelMatrix;
            glUniformMatrix4fv(escena.uMVMatrixLocation, 1, GL_FALSE, &modelViewMatrix[0][0]);
            glDrawArrays(GL_TRIANGLES, 0, num_vertices0);
            break;
        }

    } // Switch
}

//************************************************************** Clase TEscena

TEscena::TEscena() {

    seleccion = 1;
    num_objects = 0;
    num_cars = 0;

    show_car = 1;
    show_wheels = 1;
    show_road = 1;

    wireframe = 0;
    z_buffer = 1;
    culling = 0;

    scale = 100.0;
    xy_aspect = 1;
    last_x = 0;
    last_y = 0;

    memcpy(view_position, view_position_c, 3*sizeof(float));
    memcpy(view_rotate, view_rotate_c, 16*sizeof(float));

    memcpy(light0_ambient, light0_ambient_c, 4*sizeof(float));

    memcpy(light0_ambient, light0_ambient_c, 4*sizeof(float));
    memcpy(light0_diffuse, light0_diffuse_c, 4*sizeof(float));
    memcpy(light0_specular, light0_specular_c, 4*sizeof(float));
    memcpy(light0_position, light0_position_c, 4*sizeof(float));

    memcpy(light1_ambient, light1_ambient_c, 4*sizeof(float));
    memcpy(light1_diffuse, light1_diffuse_c, 4*sizeof(float));
    memcpy(light1_specular, light1_specular_c, 4*sizeof(float));
    memcpy(light1_position, light1_position_c, 4*sizeof(float));

    memcpy(mat_ambient, mat_ambient_c, 4*sizeof(float));
    memcpy(mat_diffuse, mat_diffuse_c, 4*sizeof(float));
    memcpy(mat_specular, mat_specular_c, 4*sizeof(float));
    memcpy(mat_shininess, mat_shininess_c, 1*sizeof(float));
}

void __fastcall TEscena::InitGL()
{
    int tx, ty, tw, th;

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    std::cout << "Inicializando GLEW" << std::endl << std::endl;
    if(glewInit() != GLEW_OK)
        throw std::runtime_error("glewInit failed");

    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << "Vendedor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;

    std::cout << std::endl << "Cargando Shaders" << std::endl;

    Shader shader;

    std::vector<GLuint> shaders;
    shaders.push_back(shader.LoadShader("../../Shaders/VertexShader.glsl", GL_VERTEX_SHADER));
    shaders.push_back(shader.LoadShader("../../Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER));
    shaderProgram = new Program(shaders);

    glUseProgram(shaderProgram->ReturnProgramID());

    aPositionLocation=shaderProgram->attrib(A_POSITION);
    aNormalLocation=shaderProgram->attrib(A_NORMAL);

    uProjectionMatrixLocation=shaderProgram->uniform(U_PROJECTIONMATRIX);
    uMVMatrixLocation=shaderProgram->uniform(U_MVMATRIX);
    uVMatrixLocation=shaderProgram->uniform(U_VMATRIX);
    uColorLocation=shaderProgram->uniform(U_COLOR);
    uLuz0Location=shaderProgram->uniform(U_LUZ0);

    glEnableVertexAttribArray(aPositionLocation);
    glEnableVertexAttribArray(aNormalLocation);

    GLUI_Master.get_viewport_area( &tx, &ty, &tw, &th );
    xy_aspect = (float)tw / (float)th;
    projectionMatrix = glm::perspective(45.0f, xy_aspect, 0.1f, 1000.0f);
    glUniformMatrix4fv(uProjectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
}

void __fastcall TEscena::AddCar(TPrimitiva *car)
{
    cars[num_cars] = car;
    num_cars++;
}

void __fastcall TEscena::AddObject(TPrimitiva *object)
{
    objects[num_objects] = object;
    num_objects++;
}

TPrimitiva __fastcall *TEscena::GetCar(int id)
{
    TPrimitiva *p=NULL;

    for (int i=0; i<num_cars; i++)
    {
        if (cars[i]->ID==id)
        {
            p = cars[i];
        }

    }
    return(p);
}

void __fastcall TEscena::RenderCars(bool reflejo) {

    for (int i=0; i<num_cars; i++)
    {
        cars[i]->Render(seleccion, reflejo);
    }
}

void __fastcall TEscena::RenderObjects(bool reflejo) {

    for (int i=0; i<num_objects; i++)
    {
        objects[i]->Render(seleccion, reflejo);
    }
}

void __fastcall TEscena::Render()
{
    glm::mat4 rotateMatrix;

    glClearColor(0.0, 0.7, 0.9, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    viewMatrix      = glm::mat4(1.0f);
    rotateMatrix    = glm::make_mat4(view_rotate);
    viewMatrix      = glm::translate(viewMatrix,glm::vec3(view_position[0], view_position[1], view_position[2]));
    viewMatrix      = viewMatrix*rotateMatrix;
    viewMatrix      = glm::scale(viewMatrix,glm::vec3(scale/100.0, scale/100.0, scale/100.0));

    glUniform1i(uLuz0Location, gui.light0_enabled);
    glUniformMatrix4fv(uVMatrixLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    RenderObjects(seleccion);
    RenderCars(seleccion);

    glutSwapBuffers();
}

void __fastcall TEscena::Pick3D(int mouse_x, int mouse_y)
{
}

// Funciones auxiliares para crear instancias
static TPrimitiva *NuevaCarretera(float x, float z, float giro) {
    TPrimitiva *r = new TPrimitiva(CARRETERA_ID, CARRETERA_ID);
    r->tx = x; r->tz = z; r->ry = giro; return r;
}
static TPrimitiva *NuevaFarola(float x, float z) {
    TPrimitiva *f = new TPrimitiva(FAROLA_ID, FAROLA_ID);
    f->tx = x; f->tz = z; return f;
}
static TPrimitiva *NuevaPapelera(float x, float z, float giro = 0.0f) {
    TPrimitiva *p = new TPrimitiva(PAPELERA_ID, PAPELERA_ID);
    p->tx = x; p->tz = z; p->ry = giro; return p;
}
static TPrimitiva *NuevoBanco(float x, float z, float giro = 0.0f)
{
    TPrimitiva *b = new TPrimitiva(BANCO_ID, BANCO_ID);
    b->tx = x; b->tz = z; b->ry = giro; return b;
}
static TPrimitiva *NuevaSenal(float x, float z, float giro = 0.0f) {
    TPrimitiva *s = new TPrimitiva(SENAL_ID, SENAL_ID);
    s->tx = x; s->tz = z; s->ry = giro; return s;
}
static TPrimitiva *NuevoSemaforo(float x, float z, float giro = 0.0f) {
    TPrimitiva *s = new TPrimitiva(SEMAFORO_ID, SEMAFORO_ID);
    s->tx = x; s->tz = z; s->ry = giro; return s;
}

// Función para instanciar suelos con color RGB personalizado
static TPrimitiva *NuevoSuelo(float x, float z, float ancho, float largo, float r, float g, float b) {
    TPrimitiva *s = new TPrimitiva(CRUCE_ID, CRUCE_ID);
    s->tx = x; s->tz = z; s->sx = ancho / 6.854f; s->sz = largo / 16.108f;
    s->colores[0][0] = r; s->colores[0][1] = g; s->colores[0][2] = b; s->colores[0][3] = 1.0f;
    return s;
}

static TPrimitiva *NuevoArbol(float x, float z, float giro = 0.0f) {
    TPrimitiva *a = new TPrimitiva(ARBOL_ID, ARBOL_ID);
    a->tx = x; a->tz = z; a->ry = giro; return a;
}

static TPrimitiva *NuevaParada(float x, float z, float giro = 0.0f) {
    TPrimitiva *p = new TPrimitiva(PARADA_ID, PARADA_ID);
    p->tx = x; p->tz = z; p->ry = giro; return p;
}

static TPrimitiva *NuevoQuisco(float x, float z, float giro = 0.0f) {
    TPrimitiva *q = new TPrimitiva(QUISCO_ID, QUISCO_ID);
    q->tx = x; q->tz = z; q->ry = giro; return q;
}

void __fastcall TEscena::CrearEscenario()
{
    const float A = 3.427f;          // Semianchura de la carretera
    const float H = 8.054f;          // Semilongitud de un tramo
    const float C1 = A + H;          // Centro del 1er tramo
    const float C2 = A + 3.0f * H;   // Centro del 2º tramo
    const float M  = A + 2.0f * H;   // Centro de cada manzana (19.535m)
    const float F  = A + 0.8f;       // Borde de la acera

    // 1. ASFALTO Y SUELOS =====================================================
    // Cruce central (Asfalto gris oscuro)
    TPrimitiva *cruce = new TPrimitiva(CRUCE_ID, CRUCE_ID);
    cruce->sz = A / H;
    cruce->colores[0][0] = 0.20f; cruce->colores[0][1] = 0.20f; cruce->colores[0][2] = 0.22f;
    AddObject(cruce);

    // Carreteras
    for (int i = 0; i < 2; i++) {
        float c = (i == 0) ? C1 : C2;
        AddObject(NuevaCarretera( 0,  c,  0));
        AddObject(NuevaCarretera( 0, -c,  0));
        AddObject(NuevaCarretera( c,  0, 90));
        AddObject(NuevaCarretera(-c,  0, 90));
    }

    // Suelos diferenciados por zona:
    AddObject(NuevoSuelo( M,  M, 32.2f, 32.2f, 0.75f, 0.75f, 0.78f));	// NE: Acera / Baldosa gris clara
    AddObject(NuevoSuelo(-M,  M, 32.2f, 32.2f, 0.72f, 0.52f, 0.42f));	// NW: Pavimento terracota / teja
    AddObject(NuevoSuelo( M, -M, 32.2f, 32.2f, 0.20f, 0.65f, 0.20f));	// SE: Plaza de la fuente (Suelo VERDE)
    AddObject(NuevoSuelo(-M, -M, 32.2f, 32.2f, 0.22f, 0.68f, 0.22f));	// SW: Parque (Césped verde vivo)

    // 2. REGULACIÓN DE TRÁFICO ===============================================
    AddObject(NuevoSemaforo( F, -F, 180.0f));
    AddObject(NuevoSemaforo(-F,  F,   0.0f));
    AddObject(NuevoSemaforo(-F, -F, -90.0f));
    AddObject(NuevoSemaforo( F,  F,  90.0f));

    AddObject(NuevaSenal( F, -12.0f, 180.0f));
    AddObject(NuevaSenal(-F,  12.0f,   0.0f));
    AddObject(NuevaSenal(-12.0f, -F, -90.0f));
    AddObject(NuevaSenal( 12.0f,  F,  90.0f));

    float posFarolas[] = { 10.0f, 18.0f, 26.0f };
    for (int i = 0; i < 3; i++) {
        float p = posFarolas[i];
        AddObject(NuevaFarola( F,  p)); AddObject(NuevaFarola(-F,  p));
        AddObject(NuevaFarola( F, -p)); AddObject(NuevaFarola(-F, -p));
        AddObject(NuevaFarola( p,  F)); AddObject(NuevaFarola( p, -F));
        AddObject(NuevaFarola(-p,  F)); AddObject(NuevaFarola(-p, -F));
    }

    // 3. EDIFICIOS (Ambos de color Rojo) =====================================
    TPrimitiva *edificio1 = new TPrimitiva(EDIFICIO_ID, EDIFICIO_ID);
    edificio1->tx = M; edificio1->tz = 12.0f;
    edificio1->colores[0][0] = 0.90f; edificio1->colores[0][1] = 0.20f; edificio1->colores[0][2] = 0.20f; // Rojo
    AddObject(edificio1);
    AddObject(NuevaPapelera(M - 1.5f, 5.0f));

    TPrimitiva *edificio2 = new TPrimitiva(EDIFICIO_ID, EDIFICIO_ID);
    edificio2->tx = -M; edificio2->tz = 12.0f;
    edificio2->colores[0][0] = 0.90f; edificio2->colores[0][1] = 0.20f; edificio2->colores[0][2] = 0.20f; // Mismo Rojo
    AddObject(edificio2);
    AddObject(NuevaPapelera(-M + 1.5f, 5.0f));

    // 4. PLAZA DE LA FUENTE ===================================================
    TPrimitiva *fuente = new TPrimitiva(FUENTE_ID, FUENTE_ID);
    fuente->tx = M; fuente->tz = -M;
    AddObject(fuente);

    AddObject(NuevoBanco(M,        -M + 8.5f, -90.0f));
    AddObject(NuevoBanco(M,        -M - 8.5f,   90.0f));
    AddObject(NuevoBanco(M - 8.5f, -M,         180.0f));
    AddObject(NuevoBanco(M + 8.5f, -M,        0.0f));

    AddObject(NuevaPapelera(M + 3.0f, -M + 8.5f));

    AddObject(NuevoArbol(M + 12.0f, -M + 12.0f));
    AddObject(NuevoArbol(M - 12.0f, -M + 12.0f));
    AddObject(NuevoArbol(M + 12.0f, -M - 12.0f));
    AddObject(NuevoArbol(M - 12.0f, -M - 12.0f));

    AddObject(NuevaParada(F, -M, 180.0f));

    // Kiosco en una esquina lateral de la plaza
    AddObject(NuevoQuisco(M + 11.0f, -M + 5.0f, 90.0f));

    // 5. PARQUE DEPORTIVO =====================================================
    TPrimitiva *pista = new TPrimitiva(PISTA_ID, PISTA_ID);
    pista->tx = -M; pista->tz = -M; pista->sx = 0.75f;
    AddObject(pista);

    AddObject(NuevaPapelera(-M - 6.5f, -9.0f));
    AddObject(NuevaPapelera(-M + 6.5f, -9.0f));

    AddObject(NuevoArbol(-M - 12.0f, -M + 12.0f));
    AddObject(NuevoArbol(-M + 12.0f, -M + 12.0f));
    AddObject(NuevoArbol(-M - 12.0f, -M - 12.0f));
    AddObject(NuevoArbol(-M + 12.0f, -M - 12.0f));

    // 6. COCHES ===============================================================
    TPrimitiva *car1 = new TPrimitiva(1, COCHE_ID);
    car1->tx = A / 2.0f;
    car1->tz = -C1 - 5.0f;
    car1->ry = 0.0f;
    car1->colores[0][0] = 0.95f; car1->colores[0][1] = 0.45f; car1->colores[0][2] = 0.0f; // Coche 1 Naranja

    TPrimitiva *car2 = new TPrimitiva(2, COCHE_ID);
    car2->tx = C1 + 5.0f;
    car2->tz = A / 2.0f;
    car2->ry = 90.0f;
    car2->colores[0][0] = 0.10f; car2->colores[0][1] = 0.45f; car2->colores[0][2] = 0.90f; // Coche 2 Azul

    AddCar(car1);
    AddCar(car2);
}

//************************************************************** Clase TGui

TGui::TGui()
{
    sel = 1;
    enable_panel2 = 1;
    light0_enabled = 1;
    light1_enabled = 1;
    light0_intensity = 0.8;
    light1_intensity = 0.8;
    memcpy(light0_position, light0_position_c, 4*sizeof(float));
    memcpy(light1_position, light1_position_c, 4*sizeof(float));
}

void controlCallback(int control)
{
    gui.ControlCallback(control);
}

void __fastcall TGui::Init(int main_window) {

    printf( "GLUI version: %3.2f\n", GLUI_Master.get_version() );

    window_id = main_window;

    glui = GLUI_Master.create_glui_subwindow( window_id, GLUI_SUBWINDOW_RIGHT );

    new GLUI_StaticText( glui, "" );

    GLUI_Panel *panel0 = new GLUI_Panel(glui, "Seleccion");
    GLUI_RadioGroup *radioGroup = new GLUI_RadioGroup(panel0, &sel, SEL_ID, controlCallback);
    glui->add_radiobutton_to_group(radioGroup, "NINGUNO");

    glui->add_radiobutton_to_group(radioGroup, "COCHE 1");
    glui->add_radiobutton_to_group(radioGroup, "COCHE 2");

    new GLUI_StaticText( glui, "" );

    obj_panel = new GLUI_Rollout(glui, "Propiedades", true );

    new GLUI_Checkbox( obj_panel, "Modo Alambrico", &escena.wireframe, 1, controlCallback );
    glui->add_column_to_panel(obj_panel, true);
    new GLUI_Checkbox( obj_panel, "Culling", &escena.culling, 1, controlCallback );
    new GLUI_Checkbox( obj_panel, "Z Buffer", &escena.z_buffer, 1, controlCallback );

    new GLUI_StaticText( glui, "" );

    GLUI_Rollout *roll_lights = new GLUI_Rollout(glui, "Luces", true );

    GLUI_Panel *light0 = new GLUI_Panel( roll_lights, "Luz 1" );
    GLUI_Panel *light1 = new GLUI_Panel( roll_lights, "Luz 2" );

    new GLUI_Checkbox( light0, "Encendida", &light0_enabled, LIGHT0_ENABLED_ID, controlCallback );
    light0_spinner = new GLUI_Spinner( light0, "Intensidad:", &light0_intensity,
                            LIGHT0_INTENSITY_ID, controlCallback );
    light0_spinner->set_float_limits( 0.0, 1.0 );
    GLUI_Scrollbar *sb;
    sb = new GLUI_Scrollbar( light0, "X",GLUI_SCROLL_HORIZONTAL,
                            &escena.light0_position[0],LIGHT0_POSITION_ID,controlCallback);
    sb->set_float_limits(-100,100);
    sb = new GLUI_Scrollbar( light0, "Y",GLUI_SCROLL_HORIZONTAL,
                            &escena.light0_position[1],LIGHT0_POSITION_ID,controlCallback);
    sb->set_float_limits(-100,100);
    sb = new GLUI_Scrollbar( light0, "Z",GLUI_SCROLL_HORIZONTAL,
                            &escena.light0_position[2],LIGHT0_POSITION_ID,controlCallback);
    sb->set_float_limits(-100,100);

    new GLUI_Checkbox( light1, "Encendida", &light1_enabled, LIGHT1_ENABLED_ID, controlCallback );
    light1_spinner = new GLUI_Spinner( light1, "Intensidad:", &light1_intensity,
                            LIGHT1_INTENSITY_ID, controlCallback );
    light1_spinner->set_float_limits( 0.0, 1.0 );
    sb = new GLUI_Scrollbar( light1, "X",GLUI_SCROLL_HORIZONTAL,
                            &escena.light1_position[0],LIGHT1_POSITION_ID,controlCallback);
    sb->set_float_limits(-100,100);
    sb = new GLUI_Scrollbar( light1, "Y",GLUI_SCROLL_HORIZONTAL,
                            &escena.light1_position[1],LIGHT1_POSITION_ID,controlCallback);
    sb->set_float_limits(-100,100);
    sb = new GLUI_Scrollbar( light1, "Z",GLUI_SCROLL_HORIZONTAL,
                            &escena.light1_position[2],LIGHT1_POSITION_ID,controlCallback);
    sb->set_float_limits(-100,100);

    new GLUI_StaticText( glui, "" );

    GLUI_Rollout *options = new GLUI_Rollout(glui, "Opciones", false );
    new GLUI_Checkbox( options, "Dibujar Coche", &escena.show_car );
    new GLUI_Checkbox( options, "Dibujar Ruedas", &escena.show_wheels );
    new GLUI_Checkbox( options, "Dibujar Carretera", &escena.show_road );

    new GLUI_StaticText( glui, "" );
    new GLUI_Checkbox( glui, "Permitir Movimiento", &enable_panel2 );
    new GLUI_StaticText( glui, "" );
    new GLUI_Button( glui, "Resetear Posicion", RESET_ID, controlCallback );

    new GLUI_StaticText( glui, "" );

    new GLUI_Separator( glui );

    new GLUI_StaticText( glui, "" );

    new GLUI_StaticText( glui, "  Autor:" );
    new GLUI_StaticText( glui, "  2012-2022 (C) Juan Antonio Puchol  " );

    new GLUI_StaticText( glui, "" );

    new GLUI_Separator( glui );

    new GLUI_StaticText( glui, "" );

    new GLUI_Button( glui, "Salir", 0,(GLUI_Update_CB)exit );

    glui2 = GLUI_Master.create_glui_subwindow( window_id, GLUI_SUBWINDOW_BOTTOM );

    glui->set_main_gfx_window( window_id );
    glui2->set_main_gfx_window( window_id );

    view_rot = new GLUI_Rotation(glui2, "Rotacion Escena", escena.view_rotate );
    view_rot->set_spin( 1.0 );
    new GLUI_Column( glui2, false );
    GLUI_Translation *trans_xy = new GLUI_Translation(glui2, "Traslacion Escena XY", GLUI_TRANSLATION_XY, escena.view_position );
    trans_xy->set_speed( .005 );
    new GLUI_Column( glui2, false );
    GLUI_Translation *trans_x =  new GLUI_Translation(glui2, "Traslacion Escena X", GLUI_TRANSLATION_X, escena.view_position );
    trans_x->set_speed( .005 );
    new GLUI_Column( glui2, false );
    GLUI_Translation *trans_y = new GLUI_Translation( glui2, "Traslacion Escena Y", GLUI_TRANSLATION_Y, &escena.view_position[1] );
    trans_y->set_speed( .005 );
    new GLUI_Column( glui2, false );
    GLUI_Translation *trans_z = new GLUI_Translation( glui2, "Traslacion Escena Z", GLUI_TRANSLATION_Z, &escena.scale );
    trans_z->set_speed( .005 );

}

/**************************************** TGui::ControlCallback() *******************/

void __fastcall TGui::ControlCallback( int control )
{
    switch (control) {
        case LIGHT0_ENABLED_ID: {
            if ( light0_enabled )
                light0_spinner->enable();
            else
                light0_spinner->disable();
            break;
        }
        case LIGHT1_ENABLED_ID: {
            if ( light1_enabled )
                light1_spinner->enable();
            else
                light1_spinner->disable();
            break;
        }
        case LIGHT0_INTENSITY_ID: {

            float v[] = {
                escena.light0_diffuse[0],  escena.light0_diffuse[1],
                escena.light0_diffuse[2],  escena.light0_diffuse[3] };

            v[0] *= light0_intensity;
            v[1] *= light0_intensity;
            v[2] *= light0_intensity;
            break;
        }
        case LIGHT1_INTENSITY_ID: {

            float v[] = {
                escena.light1_diffuse[0],  escena.light1_diffuse[1],
                escena.light1_diffuse[2],  escena.light1_diffuse[3] };

            v[0] *= light1_intensity;
            v[1] *= light1_intensity;
            v[2] *= light1_intensity;
            break;
        }
        case ENABLE_ID: {
            glui2->enable();
            break;
        }
        case DISABLE_ID: {
            glui2->disable();
            break;
        }
        case RESET_ID: {
            memcpy(escena.view_position,view_position_c,3*sizeof(float));
            view_rot->reset();
            escena.scale = 100.0;
            break;
        }
        case SEL_ID: {
            escena.seleccion = sel;
            glutSetWindow( glui->get_glut_window_id() );
            break;
        }
  } // switch
}

/***************************************** TGui::Idle() ***********/

void __fastcall TGui::Idle( void )
{
  if ( glutGetWindow() != window_id )
    glutSetWindow(window_id);

  if (enable_panel2)
      glui2->enable();
  else
      glui2->disable();

  glutPostRedisplay();
}

/**************************************** TGui::reshape() *************/

void __fastcall TGui::Reshape( int xx, int yy )
{
    int x, y, ancho, alto;
    GLUI_Master.get_viewport_area( &x, &y, &ancho, &alto );
    glViewport( x, y, ancho, alto );

    if (alto <= 0 || ancho <= 0) return;

    escena.xy_aspect = (float)ancho / (float)alto;
    escena.projectionMatrix = glm::perspective(45.0f, escena.xy_aspect, 0.1f, 1000.0f);
    glUniformMatrix4fv(escena.uProjectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(escena.projectionMatrix));

    glutPostRedisplay();
}

/***************************************** gui::motion() **********/

void __fastcall TGui::Motion(int x, int y )
{
    glutPostRedisplay();
}

/***************************************** gui::Mouse() **********/

void __fastcall TGui::Mouse(int button, int button_state, int x, int y )
{
    escena.Pick3D(x, y);
}
