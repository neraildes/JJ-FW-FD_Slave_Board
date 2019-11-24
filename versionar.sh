#!/bin/bash 

echo 'Iniciando processo de alteracao de versao...'

#check=$(git diff --name-only '<rev>'^! | xargs git log '<rev>'^ -1 --)

#check=$(git diff --name-only $branch^! | xargs git log $branch^ -1 --)

# check if the checkout was to checkout a branch
#if [ '$3' != '1' ]
#    then echo 'git checkout did not checkout a branch - quitting'
#fi

# get current branch name
branch=$(git rev-parse --abbrev-ref HEAD)
#check=$(git diff --name-only $branch^! | xargs git log $branch^ -1 --)

# run maven versions plugin to set new version
#mvn versions:set -DgenerateBackupPoms=false -DnewVersion=$version

echo "// arquivo criado pelo script versionar.h" > 'Liofilizador - Placa Filha.X'/versao.h
echo "#ifndef VERSAO_H"   >> 'Liofilizador - Placa Filha.X'/versao.h
echo "#define	VERSAO_H" >> 'Liofilizador - Placa Filha.X'/versao.h
echo "#define FVERSION \"$branch\"" >> 'Liofilizador - Placa Filha.X'/versao.h
echo "#endif"              >> 'Liofilizador - Placa Filha.X'/versao.h


echo "Alterado o arquivo para versao $branch."