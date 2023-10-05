(*
  initialise le plateau avec toutes les cases à 0
  Parametre :
  Return (int list list): le plateau avec toutes les cases a 0
*)
let init_plateau (size : int) : int list list =
  let rec line l x : int list =
    if x = size then
      l
    else
      line (0::l) (x+1) in
  let rec aux p y : int list list =
    if y = size then
      p
    else
      aux ((line [] 0)::p) (y+1) in
  aux [] 0;;

(*
  Affiche le plateau dans le terminal
  Parametre :
    - p (int): plateau
  Return unit
*)
let print_plateau (p : int list list) : unit =
  let rec print_line = function
    | [] -> ()
    | t::q -> print_int t; print_string " "; print_line q in
  let rec print = function
    | [] -> ()
    | t::q -> print_line t; print_string "\n"; print q in
  print p;;

print_plateau (init_plateau 5);;

(*
   Dessine un hexagone aux coordonnees x y
   Parametres
    - x (int) : position x du point le plus bas de l'hexagone
    - y (int) : position y la plus basse de l'hexagone
    Return unit
*)
let draw_hexagone i j w' : unit =
  let x = float_of_int i in
  let y = float_of_int j in
  let w = float_of_int w' in
  let c = Stdlib.cos(0.5235) in
  let s = Stdlib.sin(0.5235) in
  Graphics.moveto i j;
  Graphics.lineto (int_of_float (x -. c *. w)) (int_of_float (y +. s *. w));
  Graphics.lineto (int_of_float (x -. c *. w)) (int_of_float (y +. s *. w +. w));
  Graphics.lineto (int_of_float (x)) (int_of_float (y +. 2. *. s *. w +. w));
  Graphics.lineto (int_of_float (x +. c *. w)) (int_of_float (y +. s *. w +. w));
  Graphics.lineto (int_of_float (x +. c *. w)) (int_of_float (y +. s *. w));
  Graphics.lineto i j;;

(*
  Dessine le plateau à l'écran
  Parametres :
    - nb (int) : nombre de cases horizontales du plateau
    - s (int) : taille d'un segment des hexagones
  Return unit
*)
let draw_plateau nb s : unit =
  let sf = float_of_int s in
  let x = sf *. Stdlib.sin(1.0473) in
  let w = int_of_float(2. *. x) in
  let h = int_of_float((x /. Stdlib.tan(1.0473)) +. sf) in
  let rec line acc y =
    if acc = nb then ()
    else begin draw_hexagone ((y+1) * (int_of_float x) + acc * w) (h * y) s; line (acc+1) y end in
  let rec draw acc =
    if acc = nb then ()
    else begin line 0 acc; draw (acc+1) end in
  draw 0;;

(*
    Fonction principale
*)
let _ =
  Graphics.open_graph " 850x850+100-100";
  draw_plateau 11 30;
  ignore(Graphics.read_key());;