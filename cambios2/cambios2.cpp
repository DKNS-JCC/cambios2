#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "cambios2.h"

INT(*aQueGrupo) (INT);
VOID(*ponError) (CHAR*);
INT(*inicioCambios) (INT, HANDLE, CHAR*);
INT(*inicioCambiosHijo) (INT, HANDLE, CHAR*);


int main(int argc, char* argv[]) {
    int speed;
    if (argc != 2) {
		//velocidad maxima
        speed = 0;
        HANDLE idTimer = CreateWaitableTimer(NULL, TRUE, NULL);
        if (idTimer == NULL) {
			printf("Error al crear el timer\r\n");
			fflush(stdout);
			exit(1);
		}
        LARGE_INTEGER liDueTime;
		liDueTime.QuadPart = -200000000;
        if (!SetWaitableTimer(idTimer, &liDueTime, 0, NULL, NULL, FALSE)) {
			printf("Error al establecer el timer\r\n");
			fflush(stdout);
			exit(1);
		}
        if (WaitForSingleObject(idTimer, INFINITE) != WAIT_OBJECT_0) {
			printf("Error al esperar el timer\r\n");
			fflush(stdout);
			exit(1);
		}
	}
    else {
		speed = atoi(argv[1]);
        if (speed < 0) {
			printf("La velocidad no puede ser negativa\n");
			exit(1);
		}
        else if (speed == 0) {
            

        }
        else {
			

		}
	}

    //CARGAMOS LIBRERIA
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

    FARPROC refrescar = GetProcAddress(lib, "refrescar");
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


    //LIBERAR LIBRERIA
    if (!FreeLibrary(lib)) {
        printf("Error al liberar la libreria\r\n");
        fflush(stdout);
        exit(1);
    }


    return 0;
}