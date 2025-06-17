/****************************************************************************
 *
 *
 * Copyright (C) 2016 Universit� de Rennes 1. All rights reserved.
 *
 * This software was developed at:
 * Universite de Rennes 1
 * Campus Universitaire de Beaulieu
 * 35042 Rennes Cedex
 *
 * This file uses the ViSP library.
 *
 
*
*****************************************************************************/


/*!
tp5.cpp : Morphologie

*/

/****************************************************************************
 * NOMS - PRENOMS:
 *  - VAILLAND Damien
 *	- PLESSIS Axel
*
* Date :  25/10/2024
*****************************************************************************/

#include <iostream>

#include <visp/vpConfig.h>
#include <visp/vpDebug.h>

#include <visp/vpImage.h>
#include <visp/vpImageIo.h>
#include <visp/vpDisplayX.h>
//#include <visp3/core/vpImageConvert.h>

using namespace std;



////////////////////////////////////////////////////////////////////////////////////
////////////////////// FONCTIONS UTILES ////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

// La fonction rectangle cree une matrice binaire de taille hxw composee d'un fond noir (0) avec un rectangle
// blanc (255) dont le coin haut gauche est donne par les coordonnees i et j, et de
//largeur et longueur respectives lx et ly

vpImage<unsigned char> rectangle(int h, int w, int i, int j, int lx, int ly){
    
    vpImage<unsigned char> rect(h,w,0) ;
    int k,l;
    for(k=i;k<i+lx;k++){
        for(l=j;l<j+ly;l++){
            rect[k][l] = 255;
        }
    }
    return rect;
}


/**
 * @brief affiche une image � l'�cran � la position (posX,posY) et attend un clic
 * @param img : l'image � afficher
 * @param posX, posY : coordonn�es spatiales pour positionner affichage de la fenetre sur l'�cran
 * @param title : titre de la fenetre graphique
 * @warning : fonction bloquante
 */
void afficheImage(vpImage<unsigned char> img, int posX, int posY, const char *title)
{
    vpDisplayX d(img, posX, posY, title);
    vpDisplay::display(img);
    vpDisplay::flush(img);
    vpDisplay::getClick(img);
    vpDisplay::close(img);
}


// Fonction d'affichage d'une image sur sortie standard
void affiche(const vpImage<unsigned char> &src){
    int i,j;
    printf("\n ");
    for(i=0;i<src.getHeight();i++){
        for(j=0;j<src.getWidth();j++){
            printf("%d ",src[i][j]);
        }
        printf("\n ");
    }
}


/**
 * @brief calcule le complementaire d'une forme
 * @param im: image d'entr�e
 * @return image complementaire de im (nouvelle image, allocation memoire)
 */
void complementaire(const vpImage<unsigned char> &im, vpImage<unsigned char> &comp)
{
    
    comp.resize(im.getHeight(),im.getWidth(),0) ; //Allocate memory for an [height x width] image and initialize to 0
    
    int i,j;
    for(i=0;i<im.getHeight();i++){
        for(j=0;j<im.getWidth();j++){
            if (im[i][j]==0) comp[i][j]=255;
        }
    }
}

void symCentrale(const vpImage<unsigned char>& ES, vpImage<unsigned char>& ES_sym) {
    int rows = ES.getRows();
    int cols = ES.getCols();

    // Remplir ES_sym avec le symétrique central
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            // Inverser par rapport au centre
            ES_sym[i][j] = ES[rows - 1 - i][cols - 1 - j];
        }
    }

}



////////////////////////////////////////////////////////////////////////////////////
////////////////////// IMAGES BINAIRES  ////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////


// Cette fonction renvoie 1 (255) si l'intersection de l'element structurant es positionne au point
// (i,j) et de la forme de l'image I est non vide.
int applique_masque_dilatation(const vpImage< unsigned char > &I, const vpImage< unsigned char > &es, int i, int j )
{

    int midCol = (int) es.getCols()/2;
    int midRow = (int) es.getRows()/2;
    // std::cout << -midCol << std::endl;
    // std::cout << es.getRows() / 2 << std::endl;

    for (int i_es = -midCol ; i_es <= midCol; i_es++){
        for (int j_es = -midCol ; j_es <= midCol ; j_es++) {
            // std::cout << i+i_es << std::endl;
            // std::printf("%i / %i \n", i+i_es, j+j_es);
            if (i + i_es < 0 || j + j_es < 0 || i + i_es >= I.getHeight() || j + j_es >= I.getWidth()) {
                continue;
            }
            if (I[i + i_es][j + j_es] == 255 && es[i_es + es.getHeight() / 2][j_es + es.getWidth() / 2] == 1) {
                // std::printf("%i / %i \n", i+i_es, j+j_es);
                return 1;
            }
        }
    }
    return 0;
}


// Calcul la dilatation de l'image I par l'element structurant es.
void dilatation(const vpImage< unsigned char > &I, const vpImage< unsigned char > &es, vpImage< unsigned char > &Id)
{
    
    //Allocate memory for an [height x width] image and initialize the image to val.

    Id.resize(I.getHeight(),I.getWidth(),0) ;
    // std::printf("%i / %i \n", I.getWidth(), I.getHeight());


    for (int i = 0 ; i < I.getHeight() ; i++) {
        for (int j = 0 ; j < I.getWidth() ; j++) {

            // std::cout << test << std::endl;

            if (applique_masque_dilatation(I, es, i, j) == 1) {
                Id[i][j] = 255;
            }
            else {
                // std::cout << "pouf" << std::endl;
                Id[i][j] = 0;
            }
        }
    }

    
    

}



// Cette fonction renvoie 1 (255) si l'element structurant es positionne au point
// (i,j) de l'image est entierement inclus dans la forme de I.
int applique_masque_erosion(const vpImage< unsigned char > &I, const vpImage< unsigned char > &es, int i, int j)
{
    int midCol = (int) es.getCols()/2;
    int midRow = (int) es.getRows()/2;
    // std::cout << -midCol << std::endl;
    // std::cout << es.getRows() / 2 << std::endl;

    for (int i_es = -midCol ; i_es <= midCol; i_es++){
        for (int j_es = -midCol ; j_es <= midCol ; j_es++) {
            // std::cout << i+i_es << std::endl;
            // std::printf("%i / %i \n", i+i_es, j+j_es);
            if (i + i_es < 0 || j + j_es < 0 || i + i_es >= I.getHeight() || j + j_es >= I.getWidth()) {
                continue;
            }
            if (I[i + i_es][j + j_es] == 0 && es[i_es + es.getHeight() / 2][j_es + es.getWidth() / 2] == 1) {
            // std::printf("%i / %i \n", i+i_es, j+j_es);
                return 1;
            }
        }
    }
    return 0;
}



// Cette fonction calcule l'erosion de l'image I par l'element structurant es
void erosion(const vpImage< unsigned char > &I, const vpImage< unsigned char > &es, vpImage< unsigned char > &Ie)
{

    //Allocate memory for an [height x width] image and initialize the image to val.
    Ie.resize(I.getHeight(),I.getWidth(),0) ;
    // std::printf("%i / %i \n", I.getWidth(), I.getHeight());


    for (int i = 0 ; i < I.getHeight() ; i++) {
        for (int j = 0 ; j < I.getWidth() ; j++) {

            // std::cout << test << std::endl;

            if (applique_masque_erosion(I, es, i, j) == 1) {
                Ie[i][j] = 0;
            }
            else {
                // std::cout << "pouf" << std::endl;
                Ie[i][j] = 255;
            }
        }
    }
    

}




// Cette fonction retourne l'ouverture de l'image I par l'element structurant es
void ouverture(const vpImage< unsigned char > &I, const vpImage< unsigned char > &es, vpImage< unsigned char > &Io)
{
    vpImage< unsigned char > I_temp;
    erosion(I, es, I_temp);
    dilatation(I_temp, es, Io);   
}


// Cette fonction retourne l'ouverture de l'image I par l'element structurant es
void fermeture(const vpImage< unsigned char > &I, const vpImage< unsigned char > &es, vpImage< unsigned char > &If)
{
    vpImage< unsigned char > I_temp;
    dilatation(I, es, I_temp);
    erosion(I_temp, es, If);
}


////////////////////////////////////////////////////////////////////////////////////
//////////////////////      DUALITE     ////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////


// Cette fonction calcule l'erosion de l'image I par l'element structurant es
// en utilisant les proprietes de dualite
void erosion_dual(const vpImage< unsigned char > &I, const vpImage< unsigned char > &es, vpImage< unsigned char > &Ie)
{
    
    //Allocate memory for an [height x width] image and initialize the image to val.
    Ie.resize(I.getHeight(),I.getWidth(),0) ;

    vpImage<unsigned char> es_sym(es.getWidth(), es.getHeight(), 1);
    symCentrale(es, es_sym);
    vpImage< unsigned char > comp_I;
    vpImage< unsigned char > Id;

    complementaire(I, comp_I);
    dilatation(comp_I, es_sym, Id);
    complementaire(Id, Ie);


    

}



// Calcul la dilatation de l'image I par l'element structurant es
// en utilisant les proprietes de dualite
void dilatation_dual(const vpImage< unsigned char > &I, const vpImage< unsigned char > &es, vpImage< unsigned char > &Id)
{
    
    //Allocate memory for an [height x width] image and initialize the image to val.
    Id.resize(I.getHeight(),I.getWidth(),0) ;

    vpImage<unsigned char> es_sym(es.getWidth(), es.getHeight(), 1);
    symCentrale(es, es_sym);
    vpImage< unsigned char > comp_I;
    vpImage< unsigned char > Ie;

    complementaire(I, comp_I);
    erosion(comp_I, es_sym, Ie);
    complementaire(Ie, Id);   

}



// Cette fonction retourne l'ouverture de l'image I par l'element structurant es
// en utilisant les proprietes de dualite
void  ouverture_dual(const vpImage< unsigned char > &I, const vpImage< unsigned char > &es, vpImage< unsigned char > &Io)
{
    Io.resize(I.getHeight(),I.getWidth(),0) ;
    
    vpImage<unsigned char> es_sym(es.getWidth(), es.getHeight(), 1);
    symCentrale(es, es_sym);
    vpImage< unsigned char > comp_I;
    vpImage< unsigned char > If;

    complementaire(I, comp_I);
    fermeture(comp_I, es_sym, If);
    complementaire(If, Io);   

    
}



// Cette fonction retourne l'ouverture de l'image I par l'element structurant es
// en utilisant les proprietes de dualite
void  fermeture_dual(const vpImage< unsigned char > &I, const vpImage< unsigned char > &es, vpImage< unsigned char > &If)
{
    If.resize(I.getHeight(),I.getWidth(),0) ;
    
    vpImage<unsigned char> es_sym(es.getWidth(), es.getHeight(), 1);
    symCentrale(es, es_sym);
    vpImage< unsigned char > comp_I;
    vpImage< unsigned char > Io;

    complementaire(I, comp_I);
    ouverture(comp_I, es_sym, Io);
    complementaire(Io, If);   

    
}

////////////////////////////////////////////////////////////////////////////////////
////////////////////// IMAGES NDG       ////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////



/**
 * @brief calcule la dilatation en NDG du pixel (i,j) de l'image im par rapport a l'element structurant masque
 *
 * @param im: image de travail
 * @param masque: element structurant
 * @param i: ligne
 * @param j: colonne
 * @return valeur du point (i,j) apres dilatation
 */
int applique_masque_dilatation_NDG(const vpImage< unsigned char > &im, const vpImage< unsigned char > &masque, int i, int j){
    
    int midCol = (int) masque.getCols()/2;
    int midRow = (int) masque.getRows()/2;
    
    // affiche(masque);
    // std::printf("%i / %i \n", masque.getCols(), masque.getRows());
    

    unsigned char max_px_value = (unsigned char) 0;

    for (int i_es = -midCol ; i_es <= midCol; i_es++){
        for (int j_es = -midCol ; j_es <= midCol ; j_es++) {
            // std::printf("%i / %i \n", i_es + masque.getHeight() / 2, j_es + masque.getWidth() / 2);
            if (i + i_es < 0 || j + j_es < 0 || i + i_es >= im.getHeight() || j + j_es >= im.getWidth()) {
                continue;
            }
            if (im[i + i_es][j + j_es] > max_px_value && masque[i_es + masque.getHeight() / 2][j_es + masque.getWidth() / 2] == (unsigned char) 1) {
                max_px_value = im[i + i_es][j + j_es];
            }
        }
    }
    return max_px_value;
}



/**
 * @brief calcule l'erosion en NDG en un pixel du pixel (i,j) de l'image im selon l'element structurant masque
 *
 * @param im: image de travail
 * @param masque: element structurant
 * @param i: ligne
 * @param j: colonne
 *
 * @return valeur apres erosion au point (i,j)
 */
int applique_masque_erosion_NDG(const vpImage< unsigned char > &im, const vpImage< unsigned char > &masque, int i, int j){
    
    int midCol = (int) masque.getCols()/2;
    int midRow = (int) masque.getRows()/2;

    unsigned char min_px_value = (unsigned char) 255;

    for (int i_es = -midCol ; i_es <= midCol; i_es++){
        for (int j_es = -midCol ; j_es <= midCol ; j_es++) {
            if (i + i_es < 0 || j + j_es < 0 || i + i_es >= im.getHeight() || j + j_es >= im.getWidth()) {
                continue;
            }
            else if (masque[i_es + masque.getHeight() / 2][j_es + masque.getWidth() / 2] == 1 && im[i + i_es][j + j_es] < min_px_value) {
                min_px_value = im[i + i_es][j + j_es];
            }
        }
    }
    return min_px_value;
}


void erosion_NDG(const vpImage< unsigned char > &I, const vpImage< unsigned char > masque, vpImage< unsigned char > &Ie) {
    
    Ie.resize(I.getHeight(),I.getWidth(),0) ;

    for (int i = 0 ; i < I.getHeight() ; i++) {
        for (int j = 0 ; j < I.getWidth() ; j++) {
            Ie[i][j] = applique_masque_erosion_NDG(I, masque, i, j);
        }
    }
}

void dilatation_NDG(const vpImage< unsigned char > &I, const vpImage< unsigned char > masque, vpImage< unsigned char > &Id) {
    
    Id.resize(I.getHeight(),I.getWidth(),0) ;

    for (int i = 0 ; i < I.getHeight() ; i++) {
        for (int j = 0 ; j < I.getWidth() ; j++) {
            Id[i][j] = applique_masque_dilatation_NDG(I, masque, i, j);
        }
    }
}

void ouverture_NDG(const vpImage< unsigned char > &I, const vpImage< unsigned char > &es, vpImage< unsigned char > &Io)
{
    vpImage< unsigned char > I_temp;
    erosion_NDG(I, es, I_temp);
    dilatation_NDG(I_temp, es, Io);   
}


// Cette fonction retourne l'ouverture de l'image I par l'element structurant es
void fermeture_NDG(const vpImage< unsigned char > &I, const vpImage< unsigned char > &es, vpImage< unsigned char > &If)
{
    vpImage< unsigned char > I_temp;
    dilatation_NDG(I, es, I_temp);
    erosion_NDG(I_temp, es, If);
}



////////////////////////////////////////////////////////////////////////////////////
////////////////////// TRAITEMENTS      ////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////


// Cette fonction calcule le gradient morphologiue de l'image I avec l'element structurant es
void  gradient_morpho(const vpImage< unsigned char > &I, const vpImage< unsigned char > &es, vpImage< unsigned char > &grad)
{
    vpImage< unsigned char > Iero;
    vpImage< unsigned char > Idil;
    grad.resize(I.getHeight(),I.getWidth(),0);
    Iero.resize(I.getHeight(),I.getWidth(),0);
    Idil.resize(I.getHeight(),I.getWidth(),0);

    dilatation(I, es, Idil);
    erosion(I, es, Iero);

    grad = Idil - Iero;

}


// Cette fonction calcule le laplacien morphologique de l'image I avec l'element structurant es
void  laplacien_morpho(const vpImage< unsigned char > &I, const vpImage< unsigned char > &es, vpImage< unsigned char > &laplacien)
{
    vpImage< unsigned char > Iero;
    vpImage< unsigned char > Idil;
    laplacien.resize(I.getHeight(),I.getWidth(),0);
    Iero.resize(I.getHeight(),I.getWidth(),0);
    Idil.resize(I.getHeight(),I.getWidth(),0);

    dilatation(I, es, Idil);
    erosion(I, es, Iero);

    laplacien = (Idil - I) - (I - Iero);
    
}

void  gradient_morpho_NDG(const vpImage< unsigned char > &I, const vpImage< unsigned char > &es, vpImage< unsigned char > &grad)
{
    vpImage< unsigned char > Iero;
    vpImage< unsigned char > Idil;
    grad.resize(I.getHeight(),I.getWidth(),0);
    Iero.resize(I.getHeight(),I.getWidth(),0);
    Idil.resize(I.getHeight(),I.getWidth(),0);

    dilatation_NDG(I, es, Idil);
    erosion_NDG(I, es, Iero);

    grad = Idil - Iero;

}


// Cette fonction calcule le laplacien morphologique de l'image I avec l'element structurant es
void  laplacien_morpho_NDG(const vpImage< unsigned char > &I, const vpImage< unsigned char > &es, vpImage< unsigned char > &laplacien)
{
    vpImage< unsigned char > Iero;
    vpImage< unsigned char > Idil;
    laplacien.resize(I.getHeight(),I.getWidth(),0);
    Iero.resize(I.getHeight(),I.getWidth(),0);
    Idil.resize(I.getHeight(),I.getWidth(),0);

    dilatation_NDG(I, es, Idil);
    erosion_NDG(I, es, Iero);

    laplacien = (Idil - I) - (I - Iero);
    
}




// Cette fonction renvoit 1 (255) si le gabarit de la transformation en tout-ou-rien
// peut etre "plac�" au pixel (i,j).
int ttourien(const vpImage< unsigned char > &im, const vpImage< unsigned char > &gabarit, int i, int j){
    
    return 0;
}


// Cette fonction calcule la transformee en tout-ou-rien de l'image I par le gabarit 'gabarit'
void  toutourien(const vpImage< unsigned char > &I, const vpImage< unsigned char > &gabarit, vpImage< unsigned char > &Ittr)
{
    Ittr.resize(I.getHeight(),I.getWidth(),0);
    
    

    
}

int detecte_taille(const vpImage< unsigned char > &I){
    int compte = 0;
    for(int i=0;i<I.getHeight();i++){
        for(int j=0;j<I.getWidth();j++){
            if(I[i][j] == 255){
                compte++;
                if(I[i][j+1] == 0){
                    return compte;
                }
            }
        }
    }
    return 0 ;
}

//  Renvoie le nombre de pixel blanc dans une image I
int compte_blanc(const vpImage< unsigned char > &I){
    int compte = 0;
    for(int i=0;i<I.getHeight();i++){
        for(int j=0;j<I.getWidth();j++){
            if(I[i][j] == 255){
                compte++;
            }
        }
    }
    return compte ;
}

void compte_coin_blanc(const vpImage <unsigned char> &I, vpImage <unsigned char> &ICompt, int &cpt){
    cpt = 0;
    ICompt.resize(I.getHeight(),I.getWidth(),0);
    for(int i=0;i<I.getHeight();i++){
        for(int j=0;j<I.getWidth();j++){
            if(i>0 && j>0){
                if(I[i][j] == 255 && I[i-1][j] == 0 && I[i][j - 1] == 0){
                    ICompt[i][j] = 255;
                    cpt++;
                }
            }
        }
    }
}





////////////////////////////////////////////////////////////////////////////////////
//////////////////////      MAIN        ////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv)
{
    // selection de la fonction
    string func = "problem3";

    // creation du menu
    cout << "BINP TP 5 :  MORPHOLOGIE MATHEMATIQUE " << endl ;
    cout << "--" << endl ;
    
    // chargement d'une image
    vpImage<unsigned char>  I0;
    string sIm;
    cout << "Nom de l'image : "; cin >> sIm; // Ex: ../images/lena.pgm
    sIm = "../images/" + sIm;
    cout << sIm << endl;
    vpImageIo::read(I0,sIm) ;

    /* // A decommenter pour debugger
    vpImageIo::read(I0,"../images/lena.pgm"); */
    afficheImage(I0,100,100,"Image originale") ;
    
    // element structurant
    vpImage<unsigned char>  ES(5, 5, 1);
    
    vpImage<unsigned char> Itest=rectangle(100,200,25,50,50,100);
    afficheImage(Itest,100,500,"Rectangle test") ;

    if (func == "dilatation") {
        vpImage<unsigned char> Idilate;
        dilatation(I0,ES,Idilate);
        afficheImage(Idilate,500,500,"Image dilatee") ;
    }
    else if (func == "erosion") {
        vpImage<unsigned char> Ierode;
        erosion(I0,ES,Ierode);
        afficheImage(Ierode,500,500,"Image erodee") ;
    }
    else if (func == "ouverture") {
        vpImage<unsigned char> Iouverture;

        vpImage<unsigned char>  ESO(41,41,1);

        ouverture(I0,ESO,Iouverture);
        afficheImage(Iouverture,500,500,"Image par ouverture") ;
    }
    else if (func == "fermeture") {
        vpImage<unsigned char> Ifermeture;
        fermeture(I0,ES,Ifermeture);
        afficheImage(Ifermeture,500,500,"Image par fermeture") ;
    }
    else if (func == "Ddil") {
        vpImage<unsigned char> IDdil;
        dilatation_dual(I0, ES, IDdil);
        afficheImage(IDdil, 500, 500, "Image par dilatation dual") ;
    }
    else if (func == "Dero") {
        vpImage<unsigned char> IDero;
        erosion_dual(I0, ES, IDero);
        afficheImage(IDero, 500, 500, "Image par erosion dual") ;
    }
    else if (func == "Douv") {
        vpImage<unsigned char> IEouv;
        ouverture_dual(I0, ES, IEouv);
        afficheImage(IEouv, 500, 500, "Image par ouverture dual") ;
    }
    else if (func == "Dfer") {
        vpImage<unsigned char> IDfer;
        fermeture_dual(I0, ES, IDfer);
        afficheImage(IDfer, 500, 500, "Image par fermeture dual") ;
    }
    else if (func == "dilatation_NDG") {
        vpImage<unsigned char> Idilate;
        dilatation_NDG(I0,ES,Idilate);
        afficheImage(Idilate,500,500,"Image dilatee") ;
    }
    else if (func == "erosion_NDG") {
        vpImage<unsigned char> Ierode;
        erosion_NDG(I0,ES,Ierode);
        afficheImage(Ierode,500,500,"Image erodee") ;
    }
    else if (func == "ouverture_NDG") {
        vpImage<unsigned char> Iouv;
        ouverture_NDG(I0,ES,Iouv);
        afficheImage(Iouv,500,500,"Image par ouverture") ;
    }
    else if (func == "fermeture_NDG") {
        vpImage<unsigned char> Ifer;
        fermeture_NDG(I0,ES,Ifer);
        afficheImage(Ifer,500,500,"Image par fermeture") ;
    }
    else if (func == "gradient") {
        vpImage< uchar > grad;
        gradient_morpho(I0, ES, grad);
        afficheImage(grad,500,500,"gradient de l'image") ;
    }
    else if (func == "laplacien") {
        vpImage< uchar > laplacien;
        laplacien_morpho(I0, ES, laplacien);
        afficheImage(laplacien,500,500,"laplacien de l'image") ;
    }
    else if (func == "gradient_NDG") {
        vpImage< uchar > grad;
        gradient_morpho_NDG(I0, ES, grad);
        afficheImage(grad,500,500,"gradient de l'image") ;
    }
    else if (func == "laplacien_NDG") {
        vpImage< uchar > laplacien;
        laplacien_morpho_NDG(I0, ES, laplacien);
        afficheImage(laplacien,500,500,"laplacien de l'image") ;
    }
    else if (func == "problem1") {
        vpImage< uchar > imgCompteur;
        vpImage<unsigned char>  Masque(41, 41, 1);
        erosion(I0, Masque, imgCompteur);
        afficheImage(imgCompteur,500,500,"Reduction des carrés à 1 pixel") ;
        cout << "Nombre de carré : " << compte_blanc(imgCompteur) << endl;
    }
    else if (func == "problem2") {
        int val_masque = detecte_taille(I0);
        vpImage< uchar > imgCompteur;
        vpImage<unsigned char>  Masque(val_masque, val_masque, 1);
        cout << "Taille carré" << val_masque << endl ;
        erosion(I0, Masque, imgCompteur);
        afficheImage(imgCompteur,500,500,"Reduction des carrés à 1 pixel") ;
        cout << "Nombre de carré : " << compte_blanc(imgCompteur) << endl;
    }
    else if (func == "problem3") {
        int cpt;
        vpImage< uchar > imgCompteur;
        compte_coin_blanc(I0,imgCompteur,cpt);
        afficheImage(imgCompteur,500,500,"Reduction des carrés à 1 pixel") ;
        cout << "Nombre de carré : " << cpt << endl;
    }
    
    cout << "Fin du programme " << endl ;
    return(0);
}
