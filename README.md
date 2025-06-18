# TP5 - Morphologie Mathématique en C++ avec ViSP

> **BINP - Université de Rennes**
> 
> **Étudiants :**
> 
> - *Damien Vailland*
> - *Axel Plessis*
>   
> **Date** : 24/09/2024

## 📌 Contexte

Ce projet a été réalisé dans le cadre du cours d’Imagerie Numérique à l’ESIR (4ème année, spécialité Imagerie Numérique - IN).  
L’objectif était de mettre en œuvre les principaux opérateurs de morphologie mathématique pour le traitement d’images binaires et en niveaux de gris.

## 🎯 Objectifs pédagogiques

- Comprendre et implémenter les opérateurs morphologiques de base : **dilatation**, **érosion**, **ouverture**, **fermeture**.
- Étendre ces opérateurs aux **images en niveaux de gris**.
- Implémenter des traitements morphologiques plus avancés comme le **gradient** et le **laplacien morphologiques**.
- Appliquer ces outils pour résoudre des problèmes pratiques, notamment le **comptage de formes** dans des images binaires.

---

## 🔧 Fonctionnalités implémentées

### 1. Opérateurs de base (images binaires)

- **Dilatation** : extension des zones blanches en fonction d’un élément structurant (masque).
- **Érosion** : réduction des zones blanches.
- **Ouverture** : érosion suivie d’une dilatation. Permet d’éliminer le bruit "poivre".
- **Fermeture** : dilatation suivie d’une érosion. Permet d’éliminer le bruit "sel".
- **Dualité** : vérification expérimentale de la relation entre dilatation et érosion via le complément de l’image.

### 2. Traitement d’images en niveaux de gris

- Adaptation des opérateurs aux images contenant des valeurs d’intensité continues.
- Manipulation locale des pixels selon les valeurs maximales (dilatation) ou minimales (érosion) dans la portée du masque.

### 3. Opérateurs avancés

- **Gradient morphologique** :  
  \[(f ⊕ b) − (f ⊖ b)\]  
  Met en évidence les contours d'une image.

- **Laplacien morphologique** :  
  \[((f ⊕ b) − f) − (f − (f ⊖ b))\]  
  Fait ressortir les courbures et les zones de transition d'intensité.

### 4. Applications pratiques

- **Comptage de formes géométriques** :
  - Réduction des carrés blancs à 1 pixel via érosion pour permettre un comptage simple.
  - Identification de la taille d’un carré si inconnue.
  - Détection des coins supérieurs gauches pour le comptage unique des formes.

---

## 📸 Résultats attendus

- Visualisation des effets de chaque opérateur.
- Comparaison entre image originale et image transformée.
- Comptage précis du nombre de formes dans des cas simples et complexes.

## 📚 Conclusion
Ce TP a permis de consolider nos compétences en traitement d’images à travers l’approche de la morphologie mathématique. Les différents opérateurs implémentés s'avèrent particulièrement efficaces pour le prétraitement, l’analyse de formes, et la segmentation d’images.
Ils sont des outils fondamentaux pour de nombreuses applications concrètes en vision par ordinateur.

---

## 👨‍💻 Auteurs

- Axel Plessis  
- Damien Vailland (ESIR 2 IN)

## 🛠️ Technologies

- Langage : C++
- Bibliothèque : [ViSP (Visual Servoing Platform)](https://visp.inria.fr/)


