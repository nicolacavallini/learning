from dash import Dash, dcc, html, Input, Output

import dash_bootstrap_components as dbc

app = Dash(__name__)# external_stylesheets=[dbc.themes.BOOTSTRAP]

app.layout = html.Div([
    html.H6("Change the value in the text box to see callbacks in action!"),
    html.Div([
        "Input: ",
        dcc.Dropdown(['New York City', 'Montréal', 'San Francisco'], 'Montréal',id='my-input')
    ]),
    #html.Br(),
    html.Div(id='my-output'),

])


@app.callback(
    Output(component_id='my-output', component_property='children'),
    Input(component_id='my-input', component_property='value')
)
def update_output_div(input_value):
    return f'Output: {input_value}'


if __name__ == '__main__':
    app.run_server(debug=True)
