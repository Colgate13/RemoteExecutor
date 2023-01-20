window.addEventListener("DOMContentLoaded", () => {
  const replaceText = (selector, text) => {
    const element = document.getElementById(selector);
    if (element) element.innerText = text;
  };

  const createDiv = (text) => {
    const div = document.createElement("div");
    div.innerText = text;
    document.body.appendChild(div);
  };

  for (const type of ["chrome", "node", "electron"]) {
    replaceText(`${type}-version`, process.versions[type]);
    createDiv(`${type}-version`);
  }

});
