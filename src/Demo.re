open BsPuppeteer;
open Js_promise;

[@bs.val] external email : string = "process.env.EMAIL";
[@bs.val] external password : string = "process.env.PASSWORD";

let default = () =>
  Puppeteer.launch()
  |> then_(browser =>
       browser
       |> Browser.newPage
       |> then_(page => {
            let options =
              Navigation.makeOptions(~timeout=20000., ~waitUntil=`load, ());
            page
            |> Page.goto("https://lux.poswallet.io", ~options, ())
            |> then_(_ =>
                 page
                 |> Page.selectOneEval(
                      "#kc-form-login",
                      [%raw
                        {| function (element) { return element.setAttribute("novalidate", true); } |}
                      ],
                    )
               )
            |> then_(_ => page |> Page.type_("#username", email, ()))
            |> then_(_ => page |> Page.type_("#password", password, ()))
            |> then_(_ => page |> Page.click("#kc-login", ()))
            |> then_(_ =>
                 page
                 |> Page.waitForSelector(
                      "body > div.page-wrapper > div > div > div:nth-child(2) > div > div > div.ibox-body > div.d-flex.justify-content-between.m-b-20 > div > h3",
                      (),
                    )
               )
            |> then_(_ => {
                 Js.log("screenshotting");
                 let options =
                   Screenshot.makeOptions(~path="./screenshot.png", ());
                 page |> Page.screenshot(~options, ());
               });
          })
       |> then_(_ => Browser.close(browser))
       |> then_(_ => Node.Process.exit(0) |> resolve)
     );

default();