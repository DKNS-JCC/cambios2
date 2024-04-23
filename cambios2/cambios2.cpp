

#include <stdio.h>
#include <Windows.h>
#include "cambios2.h"

INT(*aQueGrupo) (INT);
VOID(*ponError) (CHAR);
INT(*inicioCambios) (INT, HANDLE, CHAR);
INT(*inicioCambiosHijo) (INT, HANDLE, CHAR*);

int main()
{
    HINSTANCE lib = LoadLibrary("cambios2.dll");
    if (lib == NULL)
    {
		printf("Error al cargar la libreria\n");
		return 1;
	}

	printf("Libreria cargada\n");
	FARPROC fncambios2 = GetProcAddress(lib, "fncambios2");
	if (fncambios2 == NULL)
	{
		printf("Error al cargar la funcion\n");
		return 1;
	}

	FARPROC aQuEGrupo = GetProcAddress(lib, "aQuEGrupo");
	if (aQuEGrupo == NULL)
	{
		printf("Error al cargar la funcion\n");
		return 1;
	}

    FARPROC pon_error = GetProcAddress(lib, "pon_error");
    if (pon_error == NULL)
    {
		printf("Error al cargar la funcion\n");
		return 1;
	}

    FARPROC incrementarCuenta = GetProcAddress(lib, "incrementarCuenta");
    if (incrementarCuenta == NULL)
    {
        printf("Error al cargar la funcion\n");
        return 1;
    }

    FARPROC refrescar = GetProcAddress(lib, "refrescar");
    if (refrescar == NULL)
    {
		printf("Error al cargar la funcion\n");
		return 1;
	}

    FARPROC inicioCambios = GetProcAddress(lib, "inicioCambios");
    if (inicioCambios == NULL)
    {
        printf("Error al cargar la funcion\n");
        return 1;
    }

    FARPROC inicioCambiosHijo = GetProcAddress(lib, "inicioCambiosHijo");
    if (inicioCambiosHijo == NULL)
    {
		printf("Error al cargar la funcion\n");
		return 1;
	}

    FARPROC finCambios = GetProcAddress(lib, "finCambios");
    if (finCambios == NULL)
    {
        printf("Error al cargar la funcion\n");
        return 1;
    }

    aQueGrupo = (INT(*) (INT)) GetProcAddress(lib, "aQuEGrupo");
    if (aQueGrupo == NULL) {
        printf("No se ha podido cargar la funcion\r\n");
        fflush(stdout);
        exit(1);
    }

    ponError = (VOID(*) (CHAR)) GetProcAddress(lib, "pon_error");
    if (ponError == NULL) {
        printf("No se ha podido cargar la funcion\r\n");
        fflush(stdout);
        exit(1);
    }

    inicioCambios = (INT(*) (INT, HANDLE, CHAR)) GetProcAddress(lib, "inicioCambios");
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

    if (!FreeLibrary(lib)) {
        printf("Error al liberar la libreria\r\n");
        fflush(stdout);
        exit(1);
    }

}

