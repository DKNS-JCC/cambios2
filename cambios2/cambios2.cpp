#include <Windows.h>
#include <stdio.h>
#include <list>

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
    INT velocidad;
}grupos;

typedef struct {
    int i;
}cosas;

struct ipcs {
    HANDLE mutex;
    HANDLE semaforo;
    char* memoria;
    HANDLE timer;
};

DWORD WINAPI padre(LPVOID parametro);
DWORD WINAPI hijo(LPVOID parametro);
BOOL WINAPI CtrlHandler(DWORD fdwCtrlType);

struct ipcs ipc;

int main(int argc, char* argv[]) {
    int velocidad;
    LARGE_INTEGER liDueTime;

    if (argc == 1) {
        velocidad = 0;
        liDueTime.QuadPart = -200000000LL; //20 segundos
    }
    else {
        velocidad = atoi(argv[1]);
        liDueTime.QuadPart = -300000000LL; //30 segundos
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
    ipc.mutex = CreateMutex(NULL, FALSE, "mutex");
    if (ipc.mutex == NULL) {
        printf("Error al crear el sem?foro\r\n");
        fflush(stdout);
        exit(1);
    }

    // Crear un objeto de mapeo de archivos para el archivo
    HANDLE memory_handle = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(grupos), "memoria");

    if (memory_handle == NULL) {
        // Error al crear el objeto de mapeo de archivos
        printf("Error creando memory handle");
        CloseHandle(ipc.memoria);
        return 1;
    }

    ipc.memoria = (char*)MapViewOfFile(memory_handle, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(grupos));


    if (ipc.memoria == NULL) {
        // Error al mapear la vista de archivo en la memoria
        printf("Error creando memoria");
        CloseHandle(ipc.memoria);
        return 1;
    }

    ipc.semaforo = CreateSemaphore(NULL, 0, 32, "semaforo");
    cosas parametros;
    HANDLE hThread[33];

    int indices[32];

    ((grupos*)ipc.memoria)->contador = 0;
    ((grupos*)ipc.memoria)->velocidad = velocidad;

    inicioCambios(((grupos*)ipc.memoria)->velocidad, ipc.mutex, ipc.memoria);

    //TIMER
    ipc.timer = CreateWaitableTimer(NULL, TRUE, "timer");
    if (ipc.timer == NULL) {
        printf("Error al crear el timer\r\n");
        fflush(stdout);
        exit(1);
    }

    if (!SetWaitableTimer(ipc.timer, &liDueTime, 1000, NULL, NULL, FALSE)) {
        printf("Error al establecer el timer\r\n");
        fflush(stdout);
        exit(1);
    }

    if (!SetConsoleCtrlHandler(CtrlHandler, TRUE)) {
        printf("Error al establecer el controlador de eventos\r\n");
        fflush(stdout);
        return 1;
    }

    hThread[32] = CreateThread(NULL, 0, &padre, &parametros, NULL, NULL);

    if (hThread == NULL) {
        fprintf(stderr, "Error al crear el hilo\n");
        return 1;
    }


    for (int j = 0; j < 32; j++) {
        indices[j] = j;
        hThread[j] = CreateThread(NULL, 0, &hijo, &indices[j], NULL, NULL);
    }

    //Espero timer
    WaitForSingleObject(ipc.timer, INFINITE);

    //MENSAJE FINAL
    PostThreadMessage(((grupos*)ipc.memoria)->pid[32], WM_USER + 1, 50, NULL);

    //Esperamos hijos
    WaitForMultipleObjects(33, hThread, TRUE, INFINITE);

    for (int i = 0; i < 32; i++) {
        CloseHandle(hThread[i]);
    }


    finCambios();
    if (((grupos*)ipc.memoria)->velocidad == 33) {
        printf("El programa ha sido interrumpido por el usuario (CTRL + C)\n");
    }

    CloseHandle(ipc.semaforo);
    UnmapViewOfFile(ipc.memoria);
    CloseHandle(memory_handle);
    CloseHandle(ipc.mutex);

    if (!FreeLibrary(lib)) {
        printf("Error al liberar la libreria\r\n");
        fflush(stdout);
        exit(1);
    }
    return 0;
}


DWORD WINAPI padre(LPVOID parametro) {

    MSG mensaje;
    int i = 0;
    int hijos = 0;
    int multiple[4];
    int bandera = 1;
    int fila = 0;
    int j;
    int contador;
    int remitente;
    int destinatario;
    using namespace std;
    list<int> matriz[4][4];

    PeekMessage(&mensaje, NULL, WM_USER, WM_USER + 10, PM_NOREMOVE);

    ((grupos*)ipc.memoria)->pid[32] = GetCurrentThreadId();
    ((grupos*)ipc.memoria)->personas[8].nombre = ((grupos*)ipc.memoria)->personas[9].nombre = ((grupos*)ipc.memoria)->personas[18].nombre = ((grupos*)ipc.memoria)->personas[19].nombre = ((grupos*)ipc.memoria)->personas[28].nombre = ((grupos*)ipc.memoria)->personas[29].nombre = ((grupos*)ipc.memoria)->personas[38].nombre = ((grupos*)ipc.memoria)->personas[39].nombre = ' ';

    ReleaseSemaphore(ipc.semaforo, 32, 0);
    refrescar();

    while (1) {

        GetMessage(&mensaje, NULL, WM_USER, WM_USER + 1);//tipo +0
        if (mensaje.message == WM_USER + 1) {
            break;
        }


        remitente = (mensaje.lParam) / 10;
        destinatario = (mensaje.lParam) % 10;
        if (matriz[destinatario][remitente].size() != 0) {


            PostThreadMessage(((grupos*)ipc.memoria)->pid[mensaje.wParam], WM_USER + 1, NULL, NULL);//tipo +100

            GetMessage(&mensaje, NULL, WM_USER + 3, WM_USER + 3);//tipo +200


            PostThreadMessage(((grupos*)ipc.memoria)->pid[matriz[destinatario][remitente].front()], WM_USER + 1, NULL, NULL);//tipo +100
            matriz[destinatario][remitente].pop_front();

            GetMessage(&mensaje, NULL, WM_USER + 3, WM_USER + 3);//tipo +200


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


                    PostThreadMessage(((grupos*)ipc.memoria)->pid[matriz[multiple[i] / 10][multiple[i] % 10].front()], WM_USER + 1, NULL, NULL);//tipo +100
                    matriz[multiple[i] / 10][multiple[i] % 10].pop_front();

                    GetMessage(&mensaje, NULL, WM_USER + 3, WM_USER + 3);//tipo +200
                }

            }

        }
        refrescar();

    }
    for (i = 0; i < 32; i++) {
        PostThreadMessage(((grupos*)ipc.memoria)->pid[i], WM_USER + 2, NULL, NULL);
    }

    return 0;
}

DWORD WINAPI hijo(LPVOID parametro) {

    MSG mensaje;

    int posicion;
    int flag = 1;
    int i = *((int*)parametro);
    UINT tipo;
    LONG padre;
    int j = 0;
    char nombres[32] = { 'A', 'B', 'C', 'D', 'a', 'b', 'c', 'd', 'E', 'F', 'G', 'H', 'e', 'f', 'g', 'h', 'I', 'J', 'L', 'M', 'i', 'j', 'l', 'm', 'N', 'O', 'P', 'R', 'n', 'o', 'p', 'r' };
    inicioCambiosHijo(((grupos*)ipc.memoria)->velocidad, ipc.mutex, ipc.memoria);
    PeekMessage(&mensaje, NULL, WM_USER, WM_USER + 10, PM_NOREMOVE);

    ((grupos*)ipc.memoria)->pid[i] = GetCurrentThreadId();

    //ASIGNACION DE GRUPOS

    if (i < 8) {
        posicion = i;
        ((grupos*)ipc.memoria)->personas[i].nombre = nombres[i];
        ((grupos*)ipc.memoria)->personas[i].grupo = 1;
    }
    else if (i < 16) {
        posicion = i + 2;
        ((grupos*)ipc.memoria)->personas[i + 2].nombre = nombres[i];
        ((grupos*)ipc.memoria)->personas[i + 2].grupo = 2;

    }
    else if (i < 24) {
        posicion = i + 4;
        ((grupos*)ipc.memoria)->personas[i + 4].nombre = nombres[i];
        ((grupos*)ipc.memoria)->personas[i + 4].grupo = 3;

    }
    else if (i < 32) {
        posicion = i + 6;
        ((grupos*)ipc.memoria)->personas[i + 6].nombre = nombres[i];
        ((grupos*)ipc.memoria)->personas[i + 6].grupo = 4;

    }

    WaitForSingleObject(ipc.semaforo, INFINITE);
    padre = ((grupos*)ipc.memoria)->pid[32];
    while (1) {
        ((grupos*)ipc.memoria)->personas[posicion].grupo = aQueGrupo(posicion / 10 + 1);

        tipo = posicion / 10;
        tipo = tipo * 10 + ((grupos*)ipc.memoria)->personas[posicion].grupo - 1;
        //printf("tipo %d \n",tipo);
        PostThreadMessage(padre, WM_USER, i, tipo);// tipo +0

        GetMessage(&mensaje, NULL, WM_USER + 1, WM_USER + 2);//tipo +100
        if (mensaje.message == WM_USER + 2) {
            return 0;
        }

        for (j = ((grupos*)ipc.memoria)->personas[posicion].grupo * 10 - 10; j < ((grupos*)ipc.memoria)->personas[posicion].grupo * 10; j++) {
            if (((grupos*)ipc.memoria)->personas[j].nombre == ' ') {
                ((grupos*)ipc.memoria)->personas[j].nombre = nombres[i];
                ((grupos*)ipc.memoria)->personas[j].grupo = ((grupos*)ipc.memoria)->personas[posicion].grupo;
                ((grupos*)ipc.memoria)->personas[posicion].nombre = ' ';
                posicion = j;
                break;
            }
        }

        ((grupos*)ipc.memoria)->contador++;
        incrementarCuenta();

        PostThreadMessage(padre, WM_USER + 3, i, NULL);//tipo +200
    }

    return 0;
}
BOOL WINAPI CtrlHandler(DWORD fdwCtrlType) {
    LARGE_INTEGER liDueTime;
    liDueTime.QuadPart = -0LL;
    switch (fdwCtrlType) {
        case CTRL_C_EVENT:
			((grupos*)ipc.memoria)->velocidad = 33;
            if (!SetWaitableTimer(ipc.timer, &liDueTime, 1000, NULL, NULL, FALSE)) {
				printf("Error al establecer el timer\r\n");
				fflush(stdout);
				exit(1);
			}
			return TRUE;

        default:
            return FALSE; // Indicar que no se maneja esta señal
    }
}
