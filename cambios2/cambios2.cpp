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
};

struct ipcs ipc;


DWORD WINAPI padre(LPVOID parametro) {
    MSG mensaje;
    PeekMessage(&mensaje, NULL, WM_USER, WM_USER, PM_NOREMOVE);
    int i = 0;
    int hijos = 0;
    using namespace std;
    ((grupos*)ipc.memoria)->pid[32] = GetCurrentThreadId();
    ReleaseSemaphore(ipc.semaforo, 32, 0);
    list<int> matriz[4][4];


    ((grupos*)ipc.memoria)->personas[8].nombre = ((grupos*)ipc.memoria)->personas[9].nombre = ((grupos*)ipc.memoria)->personas[18].nombre = ((grupos*)ipc.memoria)->personas[19].nombre = ((grupos*)ipc.memoria)->personas[28].nombre = ((grupos*)ipc.memoria)->personas[29].nombre = ((grupos*)ipc.memoria)->personas[38].nombre = ((grupos*)ipc.memoria)->personas[39].nombre = ' ';
    int multiple[4];
    int bandera = 1;
    int fila = 0;
    int j;

    for (hijos = 0; hijos < 32; hijos++) {
        GetMessage(&mensaje, NULL, WM_USER + 80, WM_USER + 80);
    }

    refrescar();


    UINT tipo;
    int contador;
    int remitente;
    int destinatario;

    //char nombres[32] = { 'A', 'B', 'C', 'D', 'a', 'b', 'c', 'd', 'E', 'F', 'G', 'H', 'e', 'f', 'g', 'h', 'I', 'J', 'L', 'M', 'i', 'j', 'l', 'm', 'N', 'O', 'P', 'R', 'n', 'o', 'p', 'r' };



    while (1) {

        GetMessage(&mensaje, NULL, WM_USER, WM_USER + 9);//tipo +0
        if (mensaje.message == WM_USER + 9) {
            break;
        }


        remitente = (mensaje.lParam) / 10;
        destinatario = (mensaje.lParam) % 10;
        if (matriz[destinatario][remitente].size() != 0) {
            tipo = mensaje.message + 100;


            PostThreadMessage(((grupos*)ipc.memoria)->pid[mensaje.wParam], WM_USER + 10, NULL, NULL);//tipo +100

            GetMessage(&mensaje, NULL, WM_USER + 20, WM_USER + 29);//tipo +200
            tipo = destinatario * 10 + remitente + WM_USER + 100;

            PostThreadMessage(((grupos*)ipc.memoria)->pid[matriz[destinatario][remitente].front()], WM_USER + 10, NULL, NULL);//tipo +100
            matriz[destinatario][remitente].pop_front();

            GetMessage(&mensaje, NULL, WM_USER + 20, WM_USER + 29);//tipo +200


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
                    PostThreadMessage(((grupos*)ipc.memoria)->pid[matriz[multiple[i] / 10][multiple[i] % 10].front()], WM_USER + 10, NULL, NULL);//tipo +100
                    matriz[multiple[i] / 10][multiple[i] % 10].pop_front();

                    GetMessage(&mensaje, NULL, WM_USER + 20, WM_USER + 29);//tipo +200
                }

            }

        }
        refrescar();

    }
    for (i = 0; i < 32; i++) {
        PostThreadMessage(((grupos*)ipc.memoria)->pid[i], WM_USER + 19, NULL, NULL);
    }

    return 0;
}

DWORD WINAPI hijo(LPVOID parametro) {
    MSG mensaje;
    PeekMessage(&mensaje, NULL, WM_USER, WM_USER, PM_NOREMOVE);
    int flag = 1;
    int i = *((int*)parametro);

    char nombres[32] = { 'A', 'B', 'C', 'D', 'a', 'b', 'c', 'd', 'E', 'F', 'G', 'H', 'e', 'f', 'g', 'h', 'I', 'J', 'L', 'M', 'i', 'j', 'l', 'm', 'N', 'O', 'P', 'R', 'n', 'o', 'p', 'r' };


    inicioCambiosHijo(((grupos*)ipc.memoria)->velocidad, ipc.mutex, ipc.memoria);
    ((grupos*)ipc.memoria)->pid[i] = GetCurrentThreadId();

    //ASIGNACION DE GRUPOS
    int posicion;
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
    UINT tipo;

    WaitForSingleObject(ipc.semaforo, INFINITE);

    LONG padre = ((grupos*)ipc.memoria)->pid[32];

    int j = 0;


    PostThreadMessage(padre, WM_USER + 80, NULL, NULL);// tipo +0


    while (1) {
        //printf("%d \n", mensaje.wParam);


        ((grupos*)ipc.memoria)->personas[posicion].grupo = aQueGrupo(posicion / 10 + 1);

        tipo = posicion / 10;
        tipo = tipo * 10 + ((grupos*)ipc.memoria)->personas[posicion].grupo - 1;
        //printf("tipo %d \n",tipo);
        PostThreadMessage(padre, WM_USER, i, tipo);// tipo +0


        GetMessage(&mensaje, NULL, WM_USER + 10, WM_USER + 19);//tipo +100
        if (mensaje.message == WM_USER + 19) {
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

        PostThreadMessage(padre, WM_USER + 20, i, NULL);//tipo +200

    }

    return 0;
}


int main(int argc, char* argv[]) {
    int velocidad;
    LARGE_INTEGER liDueTime;

    if (argc == 1) {
        velocidad = 0;
        liDueTime.QuadPart = -200000000LL;
    }
    else {
        velocidad = atoi(argv[1]);
        liDueTime.QuadPart = -300000000LL;
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
    ipc.mutex = CreateMutex(NULL, FALSE, "hola");
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
    ipc.semaforo = CreateSemaphore(NULL, 0, 33, "semaforo");
    cosas parametros;
    HANDLE hThread[33];

    int thread_indices[32];

    ((grupos*)ipc.memoria)->contador = 0;
    ((grupos*)ipc.memoria)->velocidad = velocidad;

    inicioCambios(((grupos*)ipc.memoria)->velocidad, ipc.mutex, ipc.memoria);

    //TIMER
    HANDLE timer = CreateWaitableTimer(NULL, TRUE, "timer");
    if (timer == NULL) {
        printf("Error al crear el timer\r\n");
        fflush(stdout);
        exit(1);
    }

    if (!SetWaitableTimer(timer, &liDueTime, 1000, NULL, NULL, FALSE)) {
        printf("Error al establecer el timer\r\n");
        fflush(stdout);
        exit(1);
    }

    hThread[32] = CreateThread(NULL, 0, &padre, &parametros, NULL, NULL);

    if (hThread == NULL) {
        fprintf(stderr, "Error al crear el hilo\n");
        return 1;
    }


    for (int j = 0; j < 32; j++) {
        thread_indices[j] = j;
        hThread[j] = CreateThread(NULL, 0, &hijo, &thread_indices[j], NULL, NULL);
    }

    WaitForSingleObject(timer, INFINITE);

    //MENSAJE FINAL
    PostThreadMessage(((grupos*)ipc.memoria)->pid[32], WM_USER + 9, 50, NULL);

    WaitForMultipleObjects(33, hThread, TRUE, INFINITE);


    finCambios();

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