open BsPuppeteer;
open Js_promise;

let default = () =>
  Puppeteer.launch()
  |> then_(browser =>
       browser
       |> Browser.newPage
       |> then_(page =>
            page
            |> Page.goto("https://lux.poswallet.io", ())
            |> then_(_ => {
                 Js.log("screenshotting");
                 let options =
                   Screenshot.makeOptions(~path="./screenshot.png", ());
                 page |> Page.screenshot(~options, ());
               })
          )
       |> then_(_ => Browser.close(browser))
       |> then_(_ => Node.Process.exit(0) |> resolve)
     );

default();