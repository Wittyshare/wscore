var casper = require('casper').create({
  verbose:false,
  logLevel:'info'
});

var url = 'http://192.168.4.22/';
var menuItems = [
'',
'About us', 
'About Steel',
'Sustainable Steel',
'Facts&Figures',
'Eurofer Stainless',
'Issues&Positions',
'News&Media',
'Contact'
]

var subMenuItems= [
'Members',
'Staff list',
'Steel Trade',
'REACH',
'Coated',
'Cold Rolled',
'Merchant',
'Rebars',
'Sections',
'Wire Rod',
'LCI Enquiries',
'Social Affairs',
'SustSteel'
]

var text = [
'Acquaviva',
'Gordon',
'Steel Trade: global flows',
'REACH: safe use of chemicals',
'Coated Sheets',
'Cold Rolled Sheet',
'Merchant Bars',
'Rebars',
'Heavy Sections',
'Wire Rod',
'Last Name',
'Social Affairs'

]

var subMenuPaths = [
  menuItems[1]+'/'+subMenuItems[0],
  menuItems[1]+'/'+subMenuItems[1],
  menuItems[2]+'/'+subMenuItems[2],
  menuItems[3]+'/Reach',
  menuItems[4]+'/Exports/'+subMenuItems[4],
  menuItems[4]+'/Exports/'+subMenuItems[5],
  menuItems[4]+'/Exports/'+subMenuItems[6],
  menuItems[4]+'/Exports/'+subMenuItems[7],
  menuItems[4]+'/Exports/'+subMenuItems[8],
  menuItems[4]+'/Exports/'+subMenuItems[9],
  menuItems[5]+'/'+subMenuItems[10],
  menuItems[6]+'/'+subMenuItems[11],
]

var ext = [
'',
'',
'.fhtml',
'.fhtml',
'.rpg',
'.rpg',
'.rpg',
'.rpg',
'.rpg',
'.rpg',
'.form',
'',
'',
''
]



casper.start(url, function() {

  //Click on each entry of the menu bar
  casper.waitForText('About us', function() {
    for(var i=1; i<8; ++i){
      casper.click('button:nth-of-type('+i+')');
      this.test.assert(
        this.getCurrentUrl() == url+menuItems[i-1], 'url is the one expected : '+menuItems[i-1]
      );
    }

    casper.click('button:nth-of-type(1)');
    casper.clickLabel('Site Map', 'span');
    this.getCurrentUrl() === url+'SiteMap', 'url is the one expected'
  
    for(var j=0; j<subMenuPaths.length; j++){

      (function(n){
        casper.waitForText(subMenuItems[n], function() {
          casper.clickLabel(subMenuItems[n], 'span');
          casper.waitForText(text[n], function() {
          this.test.assert(
            this.getCurrentUrl() === url+subMenuPaths[n]+ext[n], 'url is the one expected : ' + url+subMenuPaths[n]+ext[n]
            );
          });
        });
      })(j);

    }
  });
});

casper.then(function(){
casper.exit();
  });





casper.run();
//casper.run(function() {
//    console.log(this.getCurrentUrl()); // 'http://foo.bar/2'
//});
