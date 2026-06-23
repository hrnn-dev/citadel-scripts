#!/bin/bash

#=================================================================#
# Descripción: Cambia credenciales  de una VM usando virt-customize
#==================================================================#

#Colores para la terminal
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\044[1;33m'
NC='\033[0m' #No Color

#1. Validacion de parámetros
if [ "$#" -ne 4 ]; then
    echo -e "$RED{RED}Error de sintaxis.${NC}"
    echo -e "Uso correcto: sudo ./id_reset.sh <nombre_vm>
