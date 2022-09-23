# TP2

## Exercice 1

### 1. Pour l’image four.png fournie, de taille 100 × 100 pixels, considérons que rmin = 1, rmax = 100, δr = 2. Combien de valeurs discrètes aura-t-on pour la coordonnée r des cercles ? Et si δr = 0.5 ?

$nombre\,valeurs\,discrètes = \dfrac{rmax - rmin + 1}{δr}$

- Pour : $δr = 2$ $\implies$ $\dfrac{100 - 1 + 1}{2} = 50$

- Pour : $δr = 0.5$ $\implies$ $\dfrac{100 - 1 + 1}{0.5} = 200$

### 2. Pour la même images, en supposant que rmin = 1, rmax = 100, δr = 1, cmin = 1, cmax = 100, δc = 1,radmin = 5, radmax = 100 $\sqrt{2}$, δrad = 1, quel est le nombre total de cercles qu’on peut d´ecrire avec ces trois variables ?

$nombre\,total\,de\,cercle = \dfrac{rmax - rmin + 1}{δr} \times \dfrac{cmax - cmin + 1}{δc} \times \dfrac{radmax - radmin + 1}{δrad}$

Donc :

---

$nombre\,total\,de\,cercle = \dfrac{100 - 1 + 1}{1} \times \dfrac{100 - 1 + 1}{1} \times \dfrac{100\sqrt{2} - 5 + 1}{1} \approx 1374213$

### 3. Le tableau tridimensionnel acc associe à la case acc(i, j, k) le cercle situé à la i-ème valeur discrète de r, la j-ème valeur discrète de c, et la k-ème valeur discrète de rad. Quel est le cercle associé au acc(1, 1, 1) ? Au acc(10, 7, 30) ?

$rad^2 = (x - c)² + (y - r)²$

- $acc(1,1,1) \implies 1 = (x - 1)² + (y - 1)² $
- $acc(10,7,30) \implies 30² = (x - 7)² + (y - 10)² $

### 4. Inversement, quelle est la case de l’accumulateur associée au cercle centré dans le pixel (40,40) et de rayon rad = 13 ? Attention : les indices i,j,k doivent être entiers.

$acc(40, 40, 13)$

## Exercice 2

#### 1. Dans un premier temps un filtrage gaussien a été appliqué sur l'image d'entrée

![](/images/fourn.png)

#### 2. Afin de détecter les contours de l'image nous avons utilisé un filtre de Sobel

![](/images/sobel.png)

#### 3. Détermination de la valeur de seuil pour la détection de contour

Première tentative détecte qu'un seul grand cercle qui n'existe pas

![](/rapport/img/first_try.png)

Nous avons essayer notre algorithme avec plusieurs cercles.
Ils étaient placés de manière incohérente, avec une mauvaise taille.

![](/rapport/img/error_type.jpg)

Ce problème ne venait pas de l'algorithme en lui même mais de l'implémentation en C++.

En effet, lorsqu'on appelait la méthode pour récupérer la valeur du gradient d'un pixel on utilisait le mauvais type.
Cette mauvaise manipulation changeait la valeur :

```cpp
if ((img.at<int>(i, j)) > gradient_threshold)
```

```cpp
if ((img.at<uchar>(i, j)) > gradient_threshold)
```

Suite à la correction ci-dessus nous avions un meilleur résultat :

![](/rapport/img/sans_correct.png)

Cependant seul le plus grand cercle était détecté.

Pour pallier à ce problème on normalise les valeurs des cercles en fonction du périmètre des cercles.

```cpp
acc[r][c][radius] += 1.f/(2*M_PI*radius);
```

Suite à la normalisation nous avons pu détecter un cercle plus petit :

![](/rapport/img/correct.png)

![](/rapport/img/bruit.png)

Ensuite nous avons été confrontés au problème de détectiton de plusieurs cercles quasi-identiques en terme de position et de rayon :

![](/images/overlap.png)