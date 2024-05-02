#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <list>
#include <iostream>

INT(*refrescar) (VOID);
INT(*aQueGrupo) (INT);
VOID(*ponError) (CHAR*);
INT(*inicioCambios) (INT, HANDLE, CHAR*);
INT(*inicioCambiosHijo) (INT, HANDLE, CHAR*);
VOID(*incrementarCuenta) (VOID);


typedef struct {
    char nombre;
    char grupo;
}persona;

typedef struct {
    persona personas[40];
    int vacio;
    LONG contador;
    DWORD pid[33];
    HANDLE timer;
}grupos;

typedef struct {
    int i;
    
} cosas;

char* memoria;
HANDLE mutex;
HANDLE semaforo;

DWORD WINAPI padre(LPVOID parametro) {
    MSG mensaje;
    PeekMessage(&mensaje, NULL, WM_USER, WM_USER, PM_NOREMOVE);
    int i = 0;
    int hijos=0;
    using namespace std;
    ((grupos*)memoria)->pid[32] = GetCurrentThreadId();
    ReleaseSemaphore(semaforo,32,0);
    list<int> matriz[4][4];


    ((grupos*)memoria)->personas[8].nombre = ((grupos*)memoria)->personas[9].nombre = ((grupos*)memoria)->personas[18].nombre = ((grupos*)memoria)->personas[19].nombre = ((grupos*)memoria)->personas[28].nombre = ((grupos*)memoria)->personas[29].nombre = ((grupos*)memoria)->personas[38].nombre = ((grupos*)memoria)->personas[39].nombre = ' ';
    int multiple[4];
    int bandera = 1;
    int fila = 0;
    int j;
    
    for (hijos = 0;hijos < 32;hijos++) {
        GetMessage(&mensaje, NULL, WM_USER + 80, WM_USER + 80);
    }
    
    refrescar();

    
    UINT tipo;
    int contador;
    int remitente;
    int destinatario;
   
    //char nombres[32] = { 'A', 'B', 'C', 'D', 'a', 'b', 'c', 'd', 'E', 'F', 'G', 'H', 'e', 'f', 'g', 'h', 'I', 'J', 'L', 'M', 'i', 'j', 'l', 'm', 'N', 'O', 'P', 'R', 'n', 'o', 'p', 'r' };



    while (1) {
        
        GetMessage(&mensaje, NULL, WM_USER, WM_USER + 99);//tipo +0
        
        remitente = (mensaje.message - WM_USER) / 10;
        destinatario = (mensaje.message - WM_USER) % 10;

        if (matriz[destinatario][remitente].size() != 0) {
            tipo = mensaje.message + 100;


            PostThreadMessage(((grupos*)memoria)->pid[mensaje.wParam], tipo, NULL, NULL);//tipo +100

            GetMessage(&mensaje, NULL, tipo + 100, tipo + 100);//tipo +200
            tipo = destinatario * 10 + remitente + WM_USER + 100;

            PostThreadMessage(((grupos*)memoria)->pid[matriz[destinatario][remitente].front()], tipo, NULL, NULL);//tipo +100
            matriz[destinatario][remitente].pop_front();

            GetMessage(&mensaje, NULL, tipo + 100, tipo + 100);//tipo +200


        }
        else {

            matriz[remitente][destinatario].push_back(mensaje.wParam);
            fila = remitente;
            bandera = 1;
            contador = 0;
            while (contador < 4)
            {
                for (j = 0; j < 4; j++)
                {
                    if (matriz[fila][j].size() != 0)
                    {
                        multiple[contador] = fila * 10 + j;
                        fila = j;
                        if (j == remitente)
                        {
                            bandera = 0;
                        }
                        break;
                    }
                }
                if (bandera == 0)
                {
                    break;
                }
                contador++;


            }
            if (bandera == 0) {
                //printf("%c se quiere cambiar por %c por %c por %c \n", nombres[matriz[multiple[0] / 10][multiple[0] % 10].front()], nombres[matriz[multiple[1] / 10][multiple[1] % 10].front()], nombres[matriz[multiple[2] / 10][multiple[2] % 10].front()], nombres[matriz[multiple[3] / 10][multiple[3] % 10].front()]);
                
                for (i = 0; i <= contador; i++) {

                    tipo = multiple[i] + WM_USER + 100;
                    PostThreadMessage(((grupos*)memoria)->pid[matriz[multiple[i] / 10][multiple[i] % 10].front()], tipo, NULL, NULL);//tipo +100
                    matriz[multiple[i] / 10][multiple[i] % 10].pop_front();

                    GetMessage(&mensaje, NULL, tipo + 100, tipo + 100);//tipo +200
                }

            }

        }
        refrescar();

    }
    return 0;
}

DWORD WINAPI hijo(LPVOID parametro) {
    MSG mensaje;
    PeekMessage(&mensaje, NULL, WM_USER, WM_USER, PM_NOREMOVE);
    
    int i = *((int*)parametro);

    char nombres[32] = { 'A', 'B', 'C', 'D', 'a', 'b', 'c', 'd', 'E', 'F', 'G', 'H', 'e', 'f', 'g', 'h', 'I', 'J', 'L', 'M', 'i', 'j', 'l', 'm', 'N', 'O', 'P', 'R', 'n', 'o', 'p', 'r' };


    inicioCambiosHijo(0, mutex, memoria);
    ((grupos*)memoria)->pid[i] = GetCurrentThreadId();
    
    //ASIGNACION DE GRUPOS
    int posicion;
    if (i < 8) {
        posicion = i;
        ((grupos*)memoria)->personas[i].nombre = nombres[i];
        ((grupos*)memoria)->personas[i].grupo = 1;
    }
    else if (i < 16) {
        posicion = i + 2;
        ((grupos*)memoria)->personas[i + 2].nombre = nombres[i];
        ((grupos*)memoria)->personas[i + 2].grupo = 2;

    }
    else if (i < 24) {
        posicion = i + 4;
        ((grupos*)memoria)->personas[i + 4].nombre = nombres[i];
        ((grupos*)memoria)->personas[i + 4].grupo = 3;

    }
    else if (i < 32) {
        posicion = i + 6;
        ((grupos*)memoria)->personas[i + 6].nombre = nombres[i];
        ((grupos*)memoria)->personas[i + 6].grupo = 4;

    }
    UINT tipo;
    WaitForSingleObject(semaforo,INFINITE);
    LONG padre = ((grupos*)memoria)->pid[32];
   
    int j = 0;
    
   
    PostThreadMessage(padre, WM_USER+80, NULL, NULL);// tipo +0
  
    
    while (1) {
        //printf("%d \n", mensaje.wParam);
        
        ((grupos*)memoria)->personas[posicion].grupo = aQueGrupo(posicion / 10 + 1);
        
        tipo = posicion / 10;
        tipo = tipo * 10 + WM_USER + ((grupos*)memoria)->personas[posicion].grupo - 1;
        //printf("tipo %d \n",tipo);
        PostThreadMessage(padre, tipo, i, NULL);// tipo +0
        tipo = tipo + 100;
        GetMessage(&mensaje, NULL, tipo, tipo);//tipo +100
        tipo = tipo + 100;

        for (j = ((grupos*)memoria)->personas[posicion].grupo * 10 - 10; j < ((grupos*)memoria)->personas[posicion].grupo * 10; j++) {
            if (((grupos*)memoria)->personas[j].nombre == ' ') {
                ((grupos*)memoria)->personas[j].nombre = nombres[i];
                ((grupos*)memoria)->personas[j].grupo = ((grupos*)memoria)->personas[posicion].grupo;
                ((grupos*)memoria)->personas[posicion].nombre = ' ';
                posicion = j;
                break;
            }
        }

        ((grupos*)memoria)->contador++;
        incrementarCuenta();
        PostThreadMessage(padre, tipo, i, NULL);//tipo +200


    }
    exit(0);
}


int main(int argc, char* argv[]) {
    int vel;
    LARGE_INTEGER liDueTime;
    
    if (argc == 1) {
        puts("No se ha ingresado la velocidad de cambio, se usara la velocidad por defecto de 0");
        vel = 0;
        liDueTime.QuadPart = -200000000LL;
	}
    else {
        printf("La velocidad de cambio es %s\n", argv[1]);
		vel = atoi(argv[1]);
		liDueTime.QuadPart = 30000000LL;
	}

    HINSTANCE lib = LoadLibrary("cambios2.dll");

    if (lib == NULL) {
        printf("No se pudo cargar la libreria\r\n");
        fflush(stdout);
        exit(1);
    }

    FARPROC fnCambios = GetProcAddress(lib, "fncambios2");
    if (fnCambios == NULL) {
        printf("No se ha podido cargar la funcion\r\n");
        fflush(stdout);
        exit(1);
    }

    incrementarCuenta = (VOID(*)(VOID)) GetProcAddress(lib, "incrementarCuenta");
    if (incrementarCuenta == NULL) {
        printf("No se ha podido cargar la funcion\r\n");
        fflush(stdout);
        exit(1);
    }
    char alonso = 'A';
    refrescar = (INT(*)(VOID)) GetProcAddress(lib, "refrescar");
    if (refrescar == NULL) {
        printf("No se ha podido cargar la funcion\r\n");
        fflush(stdout);
        exit(1);


    }

    FARPROC finCambios = GetProcAddress(lib, "finCambios");
    if (finCambios == NULL) {
        printf("No se ha podido cargar la funcion\r\n");
        fflush(stdout);
        exit(1);
    }

    aQueGrupo = (INT(*) (INT)) GetProcAddress(lib, "aQuEGrupo");
    if (aQueGrupo == NULL) {
        printf("No se ha podido cargar la funcion\r\n");
        fflush(stdout);
        exit(1);
    }

    ponError = (VOID(*) (CHAR*)) GetProcAddress(lib, "pon_error");
    if (ponError == NULL) {
        printf("No se ha podido cargar la funcion\r\n");
        fflush(stdout);
        exit(1);
    }

    inicioCambios = (INT(*) (INT, HANDLE, CHAR*)) GetProcAddress(lib, "inicioCambios");
    if (inicioCambios == NULL) {
        printf("No se ha podido cargar la funcion\r\n");
        fflush(stdout);
        exit(1);
    }

    inicioCambiosHijo = (INT(*) (INT, HANDLE, CHAR*)) GetProcAddress(lib, "inicioCambiosHijo");
    if (inicioCambiosHijo == NULL) {
        printf("No se ha podido cargar la funcion\r\n");
        fflush(stdout);
        exit(1);
    }
    mutex = CreateMutex(NULL, FALSE, "hola");
    if (mutex == NULL) {
        printf("Error al crear el sem?foro\r\n");
        fflush(stdout);
        exit(1);
    }

    // Crear un objeto de mapeo de archivos para el archivo
    HANDLE memory_handle = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(grupos), "memoria");

    if (memory_handle == NULL) {
        // Error al crear el objeto de mapeo de archivos
        printf("Error creando memory handle");
        CloseHandle(memoria);
        return 1;
    }

    memoria = (char*)MapViewOfFile(memory_handle, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(grupos));


    if (memoria == NULL) {
        // Error al mapear la vista de archivo en la memoria
        printf("Error creando memoria");
        CloseHandle(memoria);
        return 1;
    }
    semaforo = CreateSemaphore(NULL, 0, 33, "semaforo");
    cosas parametros;
    HANDLE hThread;

    int thread_indices[32];

    inicioCambios(0, mutex, memoria);

    //TIMER
    HANDLE timer = CreateWaitableTimer(NULL, TRUE, "timer");
    if (timer == NULL) {
		printf("Error al crear el timer\r\n");
		fflush(stdout);
		exit(1);
	}
    ((grupos*)memoria)->timer = timer;
    
    if (!SetWaitableTimer(timer, &liDueTime, 1000, NULL, NULL, FALSE)) {
        printf("Error al establecer el timer\r\n");
        fflush(stdout);
        exit(1);
    }

    hThread = CreateThread(NULL, 0, &padre, &parametros, NULL, NULL);

    if (hThread == NULL) {
        fprintf(stderr, "Error al crear el hilo\n");
        return 1;
    }


    for (int j = 0; j < 32; j++) {
        thread_indices[j] = j;
        CreateThread(NULL, 0, &hijo, &thread_indices[j], NULL, NULL);

    }
    
    WaitForSingleObject(timer, INFINITE);
    while (1) {
        printf("a");
    }
    WaitForSingleObject(hThread, INFINITE);
    CloseHandle(hThread);
    CloseHandle(semaforo);
    CloseHandle(memory_handle);
    CloseHandle(mutex);
    if (!FreeLibrary(lib)) {
        printf("Error al liberar la libreria\r\n");
        fflush(stdout);
        exit(1);
    }
    return 0;
}