
/****************************************************************************************

    CarGL (Objects.cpp)

    Grado en Ingeniería Multimedia.
    Práctica de Gráficos por Computador.
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

float coloresc_c[2][4] = { {0.8, 0.5, 0.0, 1.0}, {0.5, 0.5, 0.5, 1.0}}; // Color del coche
float coloresr_c[2][4] = { {0.3, 0.3, 0.3, 1.0}, {1.0, 1.0, 1.0, 1.0}}; // Color de la carretera

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
	switch (tipo) {
		case CARRETERA_ID: {  // Creación de la carretera
		    tx = ty = tz = 0;

            memcpy(colores, coloresr_c, 8*sizeof(float));

            //************************ Cargar modelos 3ds ***********************************
            // formato 8 floats por vértice (x, y, z, A, B, C, u, v)
            modelo0 = Load3DS("../../Modelos/Road.3ds", &num_vertices0);
            modelo1 = Load3DS("../../Modelos/Lines.3ds", &num_vertices1);

            break;
		}
		case EDIFICIO_ID: {  // Creación del edificio
		    tx = 15.0;
		    ty = -2.5;
		    tz = 15.0;
		    rr = 0.0;

            memcpy(colores, coloresr_c, 8*sizeof(float));

            //************************ Cargar modelos 3ds ***********************************
            // formato 8 floats por vértice (x, y, z, A, B, C, u, v)
            modelo0 = Load3DS("../../Modelos/Edificio.3ds", &num_vertices0);
            modelo1 = NULL;
            break;
		}
		case COCHE_ID: { // Creación del coche
		    tx = -2.0;
		    ty =  0.8;
		    tz =  0.0;
		    rr =  0.0;

		    memcpy(colores, coloresc_c, 8*sizeof(float));

            //************************ Cargar modelos 3ds ***********************************
            // formato 8 floats por vértice (x, y, z, A, B, C, u, v)
            modelo0 = Load3DS("../../Modelos/Carroceria.3ds", &num_vertices0);
            modelo1 = Load3DS("../../Modelos/Rueda.3ds", &num_vertices1);
            break;
		}
		case FAROLA_ID: { // Creación de la farola
            tx = 0.0;
            ty = 0.0; // La base tocará el suelo
            tz = 0.0;
            ry = 0.0; // Para rotarla si fuera necesario

            // Color gris oscuro/metal
            colores[0][0] = 0.3; colores[0][1] = 0.3; colores[0][2] = 0.3; colores[0][3] = 1.0;

            //************************ Cargar modelos 3ds ***********************************
            // formato 8 floats por vértice (x, y, z, A, B, C, u, v)
            modelo0 = Load3DS("../../Modelos/Farola.3ds", &num_vertices0);
            break;
		}
		case FUENTE_ID: { // Creación de la fuente
            tx = -8.0;
            ty = 0.0;
            tz = 10.0;
            ry = 0.0;

            // Color piedra / gris claro
            colores[0][0] = 0.7; colores[0][1] = 0.7; colores[0][2] = 0.7; colores[0][3] = 1.0;

            modelo0 = Load3DS("../../Modelos/Fuente.3ds", &num_vertices0);
            modelo1 = NULL;
            break;
		}
		case PISTA_ID: { // Creación de la pista
            tx = 25.0;
            ty = 0.0;
            tz = -20.0;
            ry = 0.0;

            // Color verdoso/grisáceo para la pista
            colores[0][0] = 0.3; colores[0][1] = 0.6; colores[0][2] = 0.4; colores[0][3] = 1.0;

            modelo0 = Load3DS("../../Modelos/Pista.3ds", &num_vertices0);
            modelo1 = NULL;
            break;
        }
	} // switch
}

void __fastcall TPrimitiva::Render(int seleccion, bool reflejo)
{
    glm::mat4   modelMatrix;
    glm::mat4   modelViewMatrix;

    switch (tipo) {
        case CARRETERA_ID: {
            if (escena.show_road) {
                // Cálculo de la ModelView
                modelMatrix     = glm::mat4(1.0f);
                modelMatrix     = glm::translate(modelMatrix, glm::vec3(tx, ty, tz));
                modelMatrix     = glm::rotate(modelMatrix, (float) glm::radians(ry), glm::vec3(0,1,0)); // rotación alrededor del eje y en radianes

                modelViewMatrix = escena.viewMatrix * modelMatrix;

                // Envía nuestra ModelView al Vertex Shader
                glUniformMatrix4fv(escena.uMVMatrixLocation, 1, GL_FALSE, &modelViewMatrix[0][0]);

                // Pintar la carretera
                glUniform4fv(escena.uColorLocation, 1, colores[0]);
                glVertexAttribPointer(escena.aPositionLocation, POSITION_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo0);
                glVertexAttribPointer(escena.aNormalLocation, NORMAL_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo0+3);
                glDrawArrays(GL_TRIANGLES, 0, num_vertices0);

                // Pintar las líneas
                glUniform4fv(escena.uColorLocation, 1, colores[1]);
                glVertexAttribPointer(escena.aPositionLocation, POSITION_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo1);
                glVertexAttribPointer(escena.aNormalLocation, NORMAL_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo1+3);
                glDrawArrays(GL_TRIANGLES, 0, num_vertices1);
            }
            break;
        }

        case EDIFICIO_ID: {
            if (escena.show_road) {
                // Cálculo de la ModelView
                modelMatrix     = glm::mat4(1.0f); // matriz identidad
                modelMatrix     = glm::translate(modelMatrix, glm::vec3(tx, ty, tz));
                modelMatrix     = glm::rotate(modelMatrix, (float) glm::radians(ry), glm::vec3(0,1,0)); // rotación alrededor del eje y en radianes

                modelViewMatrix = escena.viewMatrix * modelMatrix;
                // Envía nuestra ModelView al Vertex Shader
                glUniformMatrix4fv(escena.uMVMatrixLocation, 1, GL_FALSE, &modelViewMatrix[0][0]);

                // Pintar la carretera (edificio)
                glUniform4fv(escena.uColorLocation, 1, colores[0]);
                glVertexAttribPointer(escena.aPositionLocation, POSITION_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo0);
                glVertexAttribPointer(escena.aNormalLocation, NORMAL_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo0+3);
                glDrawArrays(GL_TRIANGLES, 0, num_vertices0);
            }
            break;
        }

        case COCHE_ID: {
            // 1. Matriz base: Coloca el coche y lo gira 180º para que mire hacia adelante en la carretera
            glm::mat4 baseMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(tx, ty, tz));
            baseMatrix = glm::rotate(baseMatrix, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));

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

                // Coordenadas locales de las ruedas. ¡Nota que ahora la altura Y es -0.5f para bajarlas del chasis!
                static const float rueda[4][3] = {
                    {-0.864f, -0.5f, -1.294f}, // delantera izquierda
                    { 0.864f, -0.5f, -1.294f}, // delantera derecha
                    {-0.864f, -0.5f,  1.428f}, // trasera izquierda
                    { 0.864f, -0.5f,  1.428f}  // trasera derecha
                };

                for (int i = 0; i < 4; i++) {
                    // Usamos baseMatrix para que las ruedas hereden la posición y el giro de 180º
                    glm::mat4 modelMatrix = glm::translate(baseMatrix, glm::vec3(rueda[i][0], rueda[i][1], rueda[i][2]));
                    modelMatrix = glm::rotate(modelMatrix, (float) glm::radians(rr), glm::vec3(1.0f, 0.0f, 0.0f));

                    if (rueda[i][0] < 0) // Ruedas del lado izquierdo: girar llanta hacia fuera
                        modelMatrix = glm::rotate(modelMatrix, (float) glm::radians(180.0), glm::vec3(0.0f, 0.0f, 1.0f));

                    glm::mat4 modelViewMatrix = escena.viewMatrix * modelMatrix;
                    glUniformMatrix4fv(escena.uMVMatrixLocation, 1, GL_FALSE, &modelViewMatrix[0][0]);
                    glDrawArrays(GL_TRIANGLES, 0, num_vertices1);
                }
            }
            break;
        }

        case FAROLA_ID: {
            glUniform4fv(escena.uColorLocation, 1, (const GLfloat *) colores[0]);
            glVertexAttribPointer(escena.aPositionLocation, POSITION_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo0);
            glVertexAttribPointer(escena.aNormalLocation, NORMAL_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo0+3);

            modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(tx, ty, tz));
            modelMatrix = glm::rotate(modelMatrix, glm::radians(ry), glm::vec3(0.0f, 1.0f, 0.0f));

            modelViewMatrix = escena.viewMatrix * modelMatrix;
            glUniformMatrix4fv(escena.uMVMatrixLocation, 1, GL_FALSE, &modelViewMatrix[0][0]);
            glDrawArrays(GL_TRIANGLES, 0, num_vertices0);
            break;
        }

        case FUENTE_ID: {
            glUniform4fv(escena.uColorLocation, 1, (const GLfloat *) colores[0]);
            glVertexAttribPointer(escena.aPositionLocation, POSITION_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo0);
            glVertexAttribPointer(escena.aNormalLocation, NORMAL_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo0+3);

            modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(tx, ty, tz));
            modelMatrix = glm::rotate(modelMatrix, glm::radians(ry), glm::vec3(0.0f, 1.0f, 0.0f));

            modelViewMatrix = escena.viewMatrix * modelMatrix;
            glUniformMatrix4fv(escena.uMVMatrixLocation, 1, GL_FALSE, &modelViewMatrix[0][0]);
            glDrawArrays(GL_TRIANGLES, 0, num_vertices0);
            break;
        }

        case PISTA_ID: {
            glUniform4fv(escena.uColorLocation, 1, (const GLfloat *) colores[0]);
            glVertexAttribPointer(escena.aPositionLocation, POSITION_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo0);
            glVertexAttribPointer(escena.aNormalLocation, NORMAL_COMPONENT_COUNT, GL_FLOAT, false, STRIDE, modelo0+3);

            modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(tx, ty, tz));
            modelMatrix = glm::rotate(modelMatrix, glm::radians(ry), glm::vec3(0.0f, 1.0f, 0.0f));

            modelViewMatrix = escena.viewMatrix * modelMatrix;
            glUniformMatrix4fv(escena.uMVMatrixLocation, 1, GL_FALSE, &modelViewMatrix[0][0]);
            glDrawArrays(GL_TRIANGLES, 0, num_vertices0);
            break;
        }

    } // switch
}

//************************************************************** Clase TEscena

TEscena::TEscena() {

    seleccion = 1;
    num_objects = 0;
    num_cars = 0;

    show_car = 1;
    show_wheels = 1;
    show_road = 1;

    // live variables usadas por GLUI en TGui
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

    // Habilita el z_buffer
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Inicialización de GLEW
    std::cout << "Inicializando GLEW" << std::endl << std::endl;
    if(glewInit() != GLEW_OK)
        throw std::runtime_error("glewInit failed");

    // print out some info about the graphics drivers
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << "Vendedor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;

    // Carga de los Shaders
    std::cout << std::endl << "Cargando Shaders" << std::endl;

    Shader shader;

    std::vector<GLuint> shaders;
    shaders.push_back(shader.LoadShader("../../Shaders/VertexShader.glsl", GL_VERTEX_SHADER));
    //std::cout << "Vertex Shader: " << shader.ReturnShaderID() << std::endl;
    shaders.push_back(shader.LoadShader("../../Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER));
    //std::cout << "Fragment Shader: " << shader.ReturnShaderID() << std::endl;
    shaderProgram = new Program(shaders);

    //std::cout << "Shader Program: " << shaderProgram->ReturnProgramID() << std::endl;

    glUseProgram(shaderProgram->ReturnProgramID());
    //glValidateProgram(shaderProgram->ReturnProgramID());

    aPositionLocation=shaderProgram->attrib(A_POSITION);
    aNormalLocation=shaderProgram->attrib(A_NORMAL);

    uProjectionMatrixLocation=shaderProgram->uniform(U_PROJECTIONMATRIX);
    uMVMatrixLocation=shaderProgram->uniform(U_MVMATRIX);
    uVMatrixLocation=shaderProgram->uniform(U_VMATRIX);
    uColorLocation=shaderProgram->uniform(U_COLOR);
    uLuz0Location=shaderProgram->uniform(U_LUZ0);

    /*
    std::cout << "a_Position Location: " << aPositionLocation << std::endl;
    std::cout << "a_Normal Location: " << aNormalLocation << std::endl;

    std::cout << "u_ProjectionMatrix Location: " << uProjectionMatrixLocation << std::endl;
    std::cout << "u_MVMatrix Location: " << uMVMatrixLocation << std::endl;
    std::cout << "u_VMatrix Location: " << uVMatrixLocation << std::endl;
    std::cout << "u_Color Location: " << uColorLocation << std::endl;
    std::cout << "u_Luz0 Location: " << uLuz0Location << std::endl;
    */

    // Habilitamos el paso de attributes
    glEnableVertexAttribArray(aPositionLocation);
    glEnableVertexAttribArray(aNormalLocation);

    // Estableciendo la matriz de proyección perspectiva
    GLUI_Master.get_viewport_area( &tx, &ty, &tw, &th );
    xy_aspect = (float)tw / (float)th;
    projectionMatrix = glm::perspective(45.0f, xy_aspect, 0.1f, 1000.0f);
    glUniformMatrix4fv(uProjectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
}


/************************** TEscena::AddCar(TPrimitiva *car) *****************/

void __fastcall TEscena::AddCar(TPrimitiva *car)
{
    cars[num_cars] = car;
    num_cars++;
}

/******************** TEscena::AddObject(TPrimitiva *object) *****************/

void __fastcall TEscena::AddObject(TPrimitiva *object)
{
    objects[num_objects] = object;
    num_objects++;
}

/******************** TPrimitiva *TEscena::GetCar(int id) ********************/

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

/******************** TEscena::RenderCars() **********************************/

void __fastcall TEscena::RenderCars(bool reflejo) {

    for (int i=0; i<num_cars; i++)
    {
        cars[i]->Render(seleccion, reflejo);
    }
}

/******************** TEscena::RenderCars() **********************************/

void __fastcall TEscena::RenderObjects(bool reflejo) {

    for (int i=0; i<num_objects; i++)
    {
        objects[i]->Render(seleccion, reflejo);
    }
}

/***************************************** TEscena::Render() *****************/

void __fastcall TEscena::Render()
{
    glm::mat4 rotateMatrix;

    glClearColor(0.0, 0.7, 0.9, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Cálculo de la vista (cámara)
    viewMatrix      = glm::mat4(1.0f);
    rotateMatrix    = glm::make_mat4(view_rotate);
    viewMatrix      = glm::translate(viewMatrix,glm::vec3(view_position[0], view_position[1], view_position[2]));
    viewMatrix      = viewMatrix*rotateMatrix;
    viewMatrix      = glm::scale(viewMatrix,glm::vec3(scale/100.0, scale/100.0, scale/100.0));

    glUniform1i(uLuz0Location, gui.light0_enabled);
    glUniformMatrix4fv(uVMatrixLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix)); // Para la luz matrix view pero sin escalado!

    // Dibujar carretera y objetos
    RenderObjects(seleccion);

    // Dibujar coches
    RenderCars(seleccion);

    glutSwapBuffers();
}

// Selecciona un objeto a través del ratón
void __fastcall TEscena::Pick3D(int mouse_x, int mouse_y)
{
}

// Crea todo el escenario
void __fastcall TEscena::CrearEscenario()
{
    // ***** Instanciación de objetos *****
    TPrimitiva *road1 = new TPrimitiva(CARRETERA_ID, CARRETERA_ID);
    TPrimitiva *road2 = new TPrimitiva(CARRETERA_ID, CARRETERA_ID);

    TPrimitiva *edificio1 = new TPrimitiva(EDIFICIO_ID, EDIFICIO_ID);
    TPrimitiva *edificio2 = new TPrimitiva(EDIFICIO_ID, EDIFICIO_ID);

    TPrimitiva *farola1 = new TPrimitiva(FAROLA_ID, FAROLA_ID);
    TPrimitiva *farola2 = new TPrimitiva(FAROLA_ID, FAROLA_ID);

    TPrimitiva *car1 = new TPrimitiva(1, COCHE_ID);
    TPrimitiva *car2 = new TPrimitiva(2, COCHE_ID);

    TPrimitiva *fuente = new TPrimitiva(FUENTE_ID, FUENTE_ID);
    TPrimitiva *pista = new TPrimitiva(PISTA_ID, PISTA_ID);

    // ***** Configuración de carreteras *****
    // road1 se queda en el origen por defecto
    road2->tx = 0;
    road2->tz = 16; // Desplazada en Z para alargar el trazado

    // ***** Configuración de edificios *****
    // Edificio 1: Posición por defecto, pero se le da un tinte verdoso
    edificio1->colores[0][1]=0.8;

    // Edificio 2: Desplazado, rotado, escalado y con tinte azulado
    edificio2->tx = 27;
    edificio2->ry = 180; // Gira 180 para mirar hacia la carretera
    edificio2->sx = 2.0; // Se hace el doble de ancho en el eje X
    edificio2->colores[0][2] = 0.8;

    // ***** Configuración de farolas *****
    farola1->tx = 4.0; // Situada en la acera derecha
    farola1->tz = 5.0;

    farola2->tx = -4.0; // Situada en la acera izquierda
    farola2->tz = -5.0;

    // ***** Configuración de coches *****
    // Coche 1: Usa los valores del constructor por defecto (naranja, en tx=-2.0)

    // Coche 2: Se cambia su color a azul y se ajusta a su posición inicial
    car2->colores[0][0] = 0.1;
    car2->colores[0][1] = 0.4;
    car2->colores[0][2] = 0.9;
    car2->colores[0][3] = 1.0;
    car2->tx = 2;
    car2->tz = 3;

    // ***** Añadir a la escena para renderizar *****
    // Los objetos estáticos van a la lista principal
    AddObject(road1);
    AddObject(road2);
    AddObject(edificio1);
    AddObject(edificio2);
    AddObject(farola1);
    AddObject(farola2);
    AddObject(fuente);
    AddObject(pista);

    // Los coches usan AddCar para gestionarlos con el selector y la cámara
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

    /****************************************************/
    /*        Código con el interfaz Gráfico GLUI       */
    /****************************************************/
    printf( "GLUI version: %3.2f\n", GLUI_Master.get_version() );

    window_id = main_window;

    /*** Crea a ventana lateral ***/
    glui = GLUI_Master.create_glui_subwindow( window_id, GLUI_SUBWINDOW_RIGHT );

    // Añade una separación
    new GLUI_StaticText( glui, "" );

    // Añade un panel con texto con el valor de la selección
    GLUI_Panel *panel0 = new GLUI_Panel(glui, "Seleccion");
    GLUI_RadioGroup *radioGroup = new GLUI_RadioGroup(panel0, &sel, SEL_ID, controlCallback);
    glui->add_radiobutton_to_group(radioGroup, "NINGUNO");


    glui->add_radiobutton_to_group(radioGroup, "COCHE 1");
    glui->add_radiobutton_to_group(radioGroup, "COCHE 2");


    // Añade una separación
    new GLUI_StaticText( glui, "" );

    obj_panel = new GLUI_Rollout(glui, "Propiedades", true );

    /***** Control para las propiedades de escena *****/

    new GLUI_Checkbox( obj_panel, "Modo Alambrico", &escena.wireframe, 1, controlCallback );
    glui->add_column_to_panel(obj_panel, true);
    new GLUI_Checkbox( obj_panel, "Culling", &escena.culling, 1, controlCallback );
    new GLUI_Checkbox( obj_panel, "Z Buffer", &escena.z_buffer, 1, controlCallback );

    /******** Añade controles para las luces ********/

    // Añade una separación
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


    // Añade una separación
    new GLUI_StaticText( glui, "" );

    /***  Rollout de Opciones ***/
    GLUI_Rollout *options = new GLUI_Rollout(glui, "Opciones", false );
    new GLUI_Checkbox( options, "Dibujar Coche", &escena.show_car );
    new GLUI_Checkbox( options, "Dibujar Ruedas", &escena.show_wheels );
    new GLUI_Checkbox( options, "Dibujar Carretera", &escena.show_road );


    /*** Disable/Enable botones ***/
    // Añade una separación
    new GLUI_StaticText( glui, "" );
    new GLUI_Checkbox( glui, "Permitir Movimiento", &enable_panel2 );
    // Añade una separación
    new GLUI_StaticText( glui, "" );
    new GLUI_Button( glui, "Resetear Posicion", RESET_ID, controlCallback );

    // Añade una separación
    new GLUI_StaticText( glui, "" );

    new GLUI_Separator( glui );

     // Añade una separación
    new GLUI_StaticText( glui, "" );

    new GLUI_StaticText( glui, "  Autor:" );
    new GLUI_StaticText( glui, "  2012-2022 (C) Juan Antonio Puchol  " );

    // Añade una separación
    new GLUI_StaticText( glui, "" );

    new GLUI_Separator( glui );

    // Añade una separación
    new GLUI_StaticText( glui, "" );

    /****** A 'quit' button *****/
    new GLUI_Button( glui, "Salir", 0,(GLUI_Update_CB)exit );

    // Crea la subventana inferior
    glui2 = GLUI_Master.create_glui_subwindow( window_id, GLUI_SUBWINDOW_BOTTOM );

    /**** Link windows to GLUI, and register idle callback ******/
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
            //GLUI_Master.SetFocus(true);
            glutSetWindow( glui->get_glut_window_id() );
            break;
        }
  } // switch
}

/***************************************** TGui::Idle() ***********/

void __fastcall TGui::Idle( void )
{
  /* According to the GLUT specification, the current window is
     undefined during an idle callback.  So we need to explicitly change
     it if necessary */
  if ( glutGetWindow() != window_id )
    glutSetWindow(window_id);

  /*  GLUI_Master.sync_live_all();  -- not needed - nothing to sync in this
                                       application  */
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

    // !!!!! ATENCIÓN: comprobar que alto no sea 0, sino división por 0 !!!!!
    if (alto <= 0 || ancho <= 0) return; // Evita que explote al minimizar o cerrar

    escena.xy_aspect = (float)ancho / (float)alto;
    escena.projectionMatrix = glm::perspective(45.0f, escena.xy_aspect, 0.1f, 1000.0f);
    glUniformMatrix4fv(escena.uProjectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(escena.projectionMatrix));

    //std::cout << "xy aspect: " << escena.xy_aspect << std::endl;

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

