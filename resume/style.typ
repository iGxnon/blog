#let sidebars = state("sidebars", ())

// from chicv (the most successful cv)
#let chiline() = {
  v(-3pt);
  line(length: 100%, stroke: silver);
  v(-9pt) 
}

#let cventry(
  tl: lorem(2),
  tr: "1145/14 - 1919/8/10",
  bl: [],
  br: [],
  content
) = {
  block(
    inset: (left: 0pt),
    tl + h(1fr) + tr +
    linebreak() +
    if bl != [] or br != [] {
      bl + h(1fr) + br + linebreak()
    } +
    content
  )
}

#let main(
  avatar: none,
  body,
) = {
  set par(justify: true)
  
  set page(
   margin: (x: 0.5cm, y: 0.9cm),
   header: align(right)[
    #set text(8pt)
    #smallcaps[Last Updated on #datetime.today().display()]
   ],
   numbering: "1 / 1"
  )

  let column-gutter = 0.5cm
  let sidebar-width = 6.2cm - 0.5cm - column-gutter / 2
  let avatar-margin = 1cm
  

  let the-font = (
    "Palatino Linotype",
    "Source Han Serif SC",
    "Source Han Serif",
  )
  set list(indent: 0pt)
  set text(
    size: 9pt,
    font: the-font
  )
  
  show heading.where(
    level: 1
  ): set text(
    size: 18pt,
    weight: "light",
  )
  show heading.where(
    level: 2
  ): it => text(
    size: 12pt,
    weight: "bold",
    block(chiline() + it)
  )

  show link: it => underline(offset: 2pt, it)

  grid(
    columns: (1fr, 1pt, sidebar-width),
    column-gutter: column-gutter,
    body,
    line(angle: 90deg, length: 100%, stroke: silver, start: (0cm, sidebar-width - avatar-margin), end: (0cm, 95%)),
    locate(loc => {
      if avatar != none {
        align(center)[#image(avatar, width: sidebar-width - avatar-margin)]
      }
      for element in sidebars.final(loc) {
        element
        v(24pt, weak: true)
      }
    }),
  )
}


#let sidebar(
  body
) = sidebars.update(it => it + (body,))