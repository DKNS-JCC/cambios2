#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>

INT(*refrescar) (VOID);
INT(*aQueGrupo) (INT);
VOID(*ponError) (CHAR*);
INT(*inicioCambios) (INT, HANDLE, CHAR*);
INT(*inicioCambiosHijo) (INT, HANDLE, CHAR*);



typedef struct {
    char nombre;
    char grupo;
}persona;
typedef struct {
    persona personas[40];
    int vacio;
    LONG contador;

}grupos;

typedef struct {
    HANDLE semaphore;
    grupos* memory;

} cosas;

DWORD WINAPI padre(LPVOID parametro) {
    cosas* parametros = (cosas*)parametro;
    /*
    WaitForSingleObject(parametros->semaphore, INFINITE);

    // El hilo est? esperando en el sem?foro
    printf("El hilo padre esta esperando en el semaforo\n");

    // Simula alg?n procesamiento
    Sleep(500);

    // Libera el sem?foro
    ReleaseMutex(parametros->semaphore);
    */
    inicioCambios(0, parametros->semaphore, (char*)parametros->memory);
    parametros->memory->contador = 29;
    for (int i = 0; i < 40; i++) {
        parametros->memory->personas[i].nombre = 'A';
        parametros->memory->personas[i].grupo = 1;
    }
    refrescar();



    return 0;
}

DWORD WINAPI hijo(LPVOID parametro) {
    cosas* parametros = (cosas*)parametro;
    /*
    WaitForSingleObject(parametros->semaphore, INFINITE);

    // El hilo est? esperando en el sem?foro
    printf("El hilo hijo esta esperando en el semaforo\n");

    // Simula alg?n procesamiento
    Sleep(500);

    // Libera el sem?foro
    ReleaseMutex(parametros->semaphore);
    */
    //printf("%c", parametros->memory->personas[1].nombre);

    inicioCambiosHijo(0, parametros->semaphore, (char*)parametros->memory);

    return 0;
}

int main(int argc, char* argv[]) {
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

    FARPROC incrementarCuenta = GetProcAddress(lib, "incrementarCuenta");
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
    HANDLE semaphore = CreateMutex(NULL, FALSE, "hola");
    if (semaphore == NULL) {
        printf("Error al crear el sem?foro\r\n");
        fflush(stdout);
        exit(1);
    }


    // Crear un objeto de mapeo de archivos para el archivo
    HANDLE memoria = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(grupos), "memoria");

    if (memoria == NULL) {
        // Error al crear el objeto de mapeo de archivos
        CloseHandle(memoria);
        return 1;
    }
    grupos* ref;

    ref = (grupos*)MapViewOfFile(memoria, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(grupos));





    if (ref == NULL) {
        // Error al mapear la vista de archivo en la memoria
        CloseHandle(memoria);
        return 1;
    }

    // Ahora pMemory es un CHAR* que apunta a la zona de memoria compartida


    // Ahora pMemory apunta a la zona de memoria compartida

    cosas parametros;
    parametros.semaphore = semaphore;
    parametros.memory = ref;
    //Comprobamos si es el hijo o el padre

    //Este es el hijo
    if (argc > 1 && argv[1][0] == 'A') {


        HANDLE hThread;

        for (int i = 0; i < 32; i++) {

            hThread = CreateThread(NULL, 0, &hijo, &parametros, NULL, NULL);
            if (hThread == NULL) {
                fprintf(stderr, "Error al crear el hilo\n");
                return 1;
            }
            // Espera a que el hilo termine
            WaitForSingleObject(hThread, INFINITE);
            CloseHandle(hThread);
        }


        return 0; // Salir despu?s de imprimir el mensaje
    }
    //Este es el padre
    else if (argc > 1 && argv[1][0] == 'B') {


        HANDLE hThread = CreateThread(NULL, 0, &padre, &parametros, NULL, NULL);

        if (hThread == NULL) {
            fprintf(stderr, "Error al crear el hilo\n");
            return 1;
        }
        // Espera a que el hilo termine

        WaitForSingleObject(hThread, INFINITE);
        CloseHandle(hThread);


        return 0; // Salir despu?s de imprimir el mensaje
    }

    // Cerrar el sem?foro al finalizar
    CloseHandle(semaphore);


    // Define el nombre del ejecutable del proceso hijo y el argumento
    char command[] = "MemoriaCompartida.exe A";

    // Estructuras para almacenar informaci?n del proceso hijo
    CHAR sPath[180];
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));
    GetModuleFileName(NULL, sPath, 180);

    // Crea el proceso hijo
    if (!CreateProcess(sPath,   // Nombre del m?dulo
        command,// Comando a ejecutar
        NULL,   // Puntero a atributos de seguridad del proceso
        NULL,   // Puntero a atributos de seguridad del hilo principal
        FALSE,  // Indica si el nuevo proceso debe ser heredado
        0,      // Flags de creaci?n
        NULL,   // Puntero al entorno del nuevo proceso
        NULL,   // Directorio de inicio del nuevo proceso
        &si,    // Puntero a estructura STARTUPINFO
        &pi)) { // Puntero a estructura PROCESS_INFORMATION
        fprintf(stderr, "Error al crear el proceso hijo\n");
        return 1;
    }
    strcpy_s(command, "MemoriaCompartida.exe B");
    STARTUPINFO si2;
    PROCESS_INFORMATION pi2;
    ZeroMemory(&si2, sizeof(si2));
    si2.cb = sizeof(si2);
    ZeroMemory(&pi2, sizeof(pi2));
    if (!CreateProcess(sPath,   // Nombre del m?dulo
        command,// Comando a ejecutar
        NULL,   // Puntero a atributos de seguridad del proceso
        NULL,   // Puntero a atributos de seguridad del hilo principal
        FALSE,  // Indica si el nuevo proceso debe ser heredado
        0,      // Flags de creaci?n
        NULL,   // Puntero al entorno del nuevo proceso
        NULL,   // Directorio de inicio del nuevo proceso
        &si2,    // Puntero a estructura STARTUPINFO
        &pi2)) { // Puntero a estructura PROCESS_INFORMATION
        fprintf(stderr, "Error al crear el proceso hijo\n");
        return 1;
    }
    // Espera a que el proceso hijo termine
    WaitForSingleObject(pi.hProcess, INFINITE);
    WaitForSingleObject(pi2.hProcess, INFINITE);
    // Cierra los manejadores de proceso y hilo
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    CloseHandle(pi2.hProcess);
    CloseHandle(pi2.hThread);

    if (!FreeLibrary(lib)) {
        printf("Error al liberar la libreria\r\n");
        fflush(stdout);
        exit(1);
    }
    return 0;
}


