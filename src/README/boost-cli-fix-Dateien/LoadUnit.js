(function() {
            if (!window.Adbayes) {
		window.Adbayes = {
		 	result: 'safe',
		 	lastStamp: "194231693",
		 	isNew: false,
			properties: {},
			listeners:[]
		};
		function adbayesOnload() {
			var props = '';
			for (var key in window.Adbayes.properties) {
				props += key + '==' + Adbayes.properties[key] + '\n\n';
			}
			window.Adbayes.currentStamp = window.adbayes_stamp;
			if (!window.Adbayes.currentStamp) {
				function _getContents() {
					var contents;
					var raw = document.body.innerHTML;
					if (raw.indexOf('adbayes-content') == -1)
						contents = raw;
					else {
						contents = '';
						var elems = document.body.getElementsByClassName('adbayes-content');
						for (var i = 0; i < elems.length; i++) {
							contents += elems[i].innerHTML + ' ';
						}
					}
					contents = contents.replace(/\s+/g, ' ').replace(/^\s+|\s+$/g, '');
					return contents;
				};
				String.prototype.hashCode = function(){
					var hash = 0;
					if (this.length == 0) return hash;
					for (i = 0; i < this.length; i++) {
						char = this.charCodeAt(i);
						hash = ((hash<<5)-hash)+char;
						hash = hash & hash;
					}
					return String(hash);
				}
				window.Adbayes.content = _getContents();
				var hashText = props + window.Adbayes.content.replace(/<[^>]+>|\s+/g, '');
				window.Adbayes.currentStamp = hashText.hashCode();
			}
			window.Adbayes.isOutdated = window.Adbayes.currentStamp != window.Adbayes.lastStamp;
			var isWaiting = false;
                    if (window.Adbayes.isOutdated || isWaiting) {
				window.Adbayes.content = !window.Adbayes.content? _getContents() : window.Adbayes.content;
				var data =
					"c=1"+
					"&url="+encodeURIComponent("http://boost.2283326.n4.nabble.com/thread-on-tls-prepare-in-win32-tss-pe-cpp-incompatible-with-static-linkage-to-a-clr-object-td2660384.html")+
					"&stamp="+encodeURIComponent(window.Adbayes.currentStamp)+
					"&rawHTML="+encodeURIComponent(window.Adbayes.content)+
					"&props="+encodeURIComponent(props);
				try {
					if (window.XDomainRequest) {
						var xdr = new XDomainRequest();
						xdr.onload = function() { window.Adbayes.sent = true; };
						xdr.onerror = function() { window.Adbayes.error = xdr.responseText; };
						xdr.onprogress = function() {};
						xdr.open("POST", "http://www.adbayes.com/Update.jtp");
						xdr.send(data);
					} else {
						var xhr = new XMLHttpRequest();
						xhr.onreadystatechange = function() {
							if (xhr.readyState==4 && xhr.status==200)
								window.Adbayes.sent = true;
						};
						xhr.open("POST", "http://www.adbayes.com/Update.jtp", true);
						xhr.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
						xhr.send(data);
					}
				} catch(err) {
					window.Adbayes.error = err;
				}
			}
			for (var i = 0; i < window.Adbayes.listeners.length; i++) {
				window.Adbayes.listeners[i]();
			}
		};
		if (window.addEventListener) {
			window.addEventListener("load", adbayesOnload, false);
		} else if (window.attachEvent) {
			window.attachEvent("onload", adbayesOnload);
		} else {
		   document.addEventListener("load", adbayesOnload, false);
		}
            }
})();