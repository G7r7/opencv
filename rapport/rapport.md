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

#### Filtrage gaussien

<img title="" src="file:///home/germain/dev/opencv/images/fourn.png" alt="" width="152" data-align="inline"><img title="" src="file:///home/germain/dev/opencv/images/gaussien.png" alt="" data-align="inline">

#### Sobel

<img src="file:///home/germain/dev/opencv/images/fourn.png" title="" alt="" width="148">![](/home/germain/dev/opencv/images/sobel.png)

#### Détermination de la valeur de seuil pour la détection de contour
