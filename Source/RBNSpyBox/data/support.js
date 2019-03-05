function updateNameVersion() {

    // Get the program name and version

    $.ajax({
        url: "getNameVersion"
    }).done(function (data) {

    	// We have the data, process it.

        var indexNameVersionETC = data.documentElement;
        $("#programName").html(indexNameVersionETC.getAttribute("Name"));
        $("#programVersion").html(indexNameVersionETC.getAttribute("Version"));
        $("#programCopyright").html(indexNameVersionETC.getAttribute("Copyright"));

        document.title = indexNameVersionETC.getAttribute("Name");

    }).fail(function (jqXHR, textStatus) {

        alert("Contact with host lost!" + textStatus);
    });
}

// Home.html specific

// This method is called to add a number of table headers to a table object.
// It is passed the div into which to draw and it will create the new table
// and return a reference to the associated <tbody>.

function buildTable(divToUse, titles) {

    // Clean out the div

    divToUse.empty();

    // Create the main table objects

    var table = $("<table/>");
    var thead = $("<thead/>");
    var tbody = $("<tbody/>");
    var tr = $("<tr/>");

    // Append them together

    tr.appendTo(thead);
    thead.appendTo(table);
    tbody.appendTo(table);

    // Process and add all the table headers

    for (var i = 0; i < titles.length; i++) {
        var th = $("<th/>");
        var span = $("<span/>");
        span.html(titles[i]);
        span.appendTo(th);

        th.appendTo(tr);
    }

    // Append them to the table

    table.appendTo(divToUse);

    // Return the table body

    return tbody;
}

// Settings.html specific

function updateSSIDs() {

    // Get the program name and version

    $.ajax({
        url: "getSSIDs"
    }).done(function (data) {

        // We have the data, process it.

        $("#availableSSIDS").empty();

        var ssids = data.documentElement.getElementsByTagName("SSID");
        if (0 === ssids.length) {
            alert("No SSIDS visible!");
        }
        else {

            for (var i = 0; i < ssids.length; i++) {

                var ssid = ssids[i];

                $("<option/>").val(ssid.getAttribute("Name")).html(ssid.getAttribute("Name")).appendTo("#availableSSIDS");
            }
        }

    }).fail(function (jqXHR, textStatus) {

        alert("Contact with host lost!" + textStatus);
    });
}