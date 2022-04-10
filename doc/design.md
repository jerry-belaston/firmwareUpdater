# SDD Overview

Here a light **S**oftware **D**esign **D**ocument giving the main lines of the Firmware Updater application architecture.

## External Dependencies

C++ based application depending on Qt Framework and Open SSL library on Windows (needed to be able to request on the network).

## Layers definition

Light implementation of MVC-VM (Model-View-Controller, View-Model) pattern organized as following:

- `core` namespace:
    - `model`: namespace in charge of the whole model. For now, it only manages the mock server.
	- `controller::Controller`: class managing the model and the view, abstracted way, which allows to split ui/core dependencies. For now, we don't really manage view model since in this light application we don't need to restore last view states.
    - `common`: helpers and types used by the whole application (no matter the layer)
    - `toolkit`: helpers mainly implemented using c++ Standard Library. For filesystemm or Json management, Qt has been used to be fast but should not.
- `ui` namespace:
	- Qt widgets concrete views implementing `controller::ViewInterface`.
    - `toolkit`: custom qt widgets/helpers.

## JSON templates file schema

No JSON schema validator is used for now. Therefore, let's just give an extract of JSON file containing only one template with one step:
```
{
    "templates": [
        {
            "name": "Default",
            "steps": [
                {
                    "button_next": {
                        "text": "Next"
                    },
                    "button_previous": {
                        "displayed": true,
                        "text": "Previous"
                    },
                    "description": {
                        "color": "black",
                        "size": 16,
                        "text": "First step"
                    },
                    "image_url": "https://upload.wikimedia.org/wikipedia/commons/5/54/Light_shining1.JPG"
                }
            ]
        }
    ],
    "version": 1
}
```

## License

Copyright (C) 2022, Jerry Belaston