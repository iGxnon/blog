#import "fontawesome.typ": *;

#let iconlink(uri, icon: link-icon, text: []) = {
  if text == [] {
    text = uri
  }
  link(uri)[#fa[#icon] #text]
}

#let githublink(userRepo) = {
  link("https://github.com/" + userRepo)[#fa[#github] #userRepo]
}
