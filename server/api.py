# -*- coding: utf-8 -*-
"""Progetto Ambiente dell'Accademia dell'Hardware e del Software Libero A. Olivetti.

Bozza di interfaccia web per la fruizione dei dati.

https://github.com/ma1069/progetto-ambiente#readme
"""

import os
import sqlite3
import json
from datetime import datetime
from flask import Flask, request, render_template, g, jsonify, make_response
from flask_restful import reqparse, abort, Api, Resource


# Percorso del database sqlite
DATABASE = os.path.dirname(__file__) + '/progamb.db'


# Istanza della app Flask e della API REST
app = Flask(__name__)
api = Api(app)


#measures = {}

# Parser dei parametri passati via URL nelle chiamate all'API
parser = reqparse.RequestParser()
parser.add_argument('device_id')
parser.add_argument('date_time')
parser.add_argument('parameter')
parser.add_argument('value')
parser.add_argument('values')


# Funzioni di utilità per semplificare le query (da https://flask.palletsprojects.com/en/2.0.x/patterns/sqlite3/)

def make_dicts(cursor, row):
    return dict((cursor.description[idx][0], value) for idx, value in enumerate(row))


def get_db():
    db = getattr(g, '_database', None)
    if db is None:
        db = g._database = sqlite3.connect(DATABASE)
        #db.row_factory = sqlite3.Row
        db.row_factory = make_dicts
    return db


@app.teardown_appcontext
def close_connection(exception):
    db = getattr(g, '_database', None)
    if db is not None:
        db.close()


def query_db(query, args=(), one=False):
    cur = get_db().execute(query, args)
    rv = cur.fetchall()
    cur.close()
    return (rv[0] if rv else None) if one else rv


# API "ping" per verificare che l'API sia in funzione lato server
class CheckAlive(Resource):
    def get(self):
        return {'status': 'ok', 'message': 'pong'}, 200


# API "measure_id" per operare con una singola misura (lettura, modifica, cancellazione)
class Measure(Resource):
    def get(self, measure_id):
        m = query_db('select * from measure where id=?', [measure_id], one=True)
        if m is None:
            return {}, 404
        else:
            return m, 200

    def delete(self, measure_id):
        c = get_db().execute('delete from measure where id=?', [measure_id])
        if c.rowcount > 0:
            get_db().commit()
            return {'status': 'ok'}, 204
        else:
            return {'status': 'fail', 'reason': 'not found'}, 404

    def put(self, measure_id):
        args = parser.parse_args()

        d = query_db('select * from device where id=?', [args['device_id']], one=True)
        if d is None:
            return {'status': 'fail', 'reason': 'unknown device'}, 404

        device_id = args['device_id']
        date_time = args['date_time']
        parameter = args['parameter']
        value = args['value']

        r = get_db().execute('insert or replace into measure (id, device_id, date_time, parameter, value) values (?, ?, ?, ?, ?)', [measure_id, device_id, date_time, parameter, value])
        if r.rowcount > 0:
            get_db().commit()
            return {'status': 'ok'}, 200
        else:
            return {'status': 'fail', 'reason': 'not found'}, 404


# API "device" per operare con tutte le misure ricevute da un dato device (lista, inserimento massivo)
class MeasureList(Resource):
    def get(self, device_id):
        d = query_db('select * from device where id=?', [device_id], one=True)
        if d is None:
            return {'status': 'fail', 'reason': 'unknown device'}, 404

        measures = query_db('select * from measure where device_id=?', [device_id])
        return measures, 200

    def post(self, device_id):
        args = parser.parse_args()

        d = query_db('select * from device where id=?', [device_id], one=True)
        if d is None:
            return {'status': 'fail', 'reason': 'unknown device'}, 404

        date_time = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        fail = False
        for item in args['values'].split('|'):
            parameter, value = item.split(':')
            r = get_db().execute('insert into measure (device_id, date_time, parameter, value) values (?, ?, ?, ?)', [device_id, date_time, parameter, value])
            if r.rowcount == 0:
                fail = True
                break
        if not fail:
            get_db().commit()
            return {'status': 'ok'}, 200
        else:
            return {'status': 'fail', 'reason': 'unable to store one or more measures'}, 400


# Associa le API con le corrispondenti URL
api.add_resource(CheckAlive, '/v0/data/ping')
api.add_resource(Measure, '/v0/data/<string:measure_id>')
api.add_resource(MeasureList, '/v0/data/device/<string:device_id>')


# Visualizzazione delle misura su mappa OSM
@app.route("/")
def map():
    measures = {}
    for m in query_db('select device.name,'
                      'device.description as meas_desc,'
		      'device.lat,'
		      'device.lng,'
		      'measure.id,'
		      'measure.device_id,'
		      'max(measure.date_time) as latest,'
		      'measure.parameter,'
		      'measure.value,'
		      'parameter.name as param_name,'
		      'parameter.description as param_desc,'
		      'parameter.unit '
		      'from measure '
		      'left join device on device.id = measure.device_id '
		      'left join parameter on measure.parameter = parameter.name '
		      'group by parameter'):
        if m['device_id'] not in measures:
            measures[m['device_id']] = {
                'name': m['name'],
                'description': m['meas_desc'],
                'lat': m['lat'],
                'lng': m['lng'],
                'date_time': m['latest'],
                'last_measures': [ {'parameter': m['param_name'], 'description': m['param_desc'], 'value': m['value'], 'unit': m['unit']} ]
            }
        else:
            measures[m['device_id']]['last_measures'].append({'parameter': m['param_name'], 'description': m['param_desc'], 'value': m['value'], 'unit': m['unit']})
    return render_template('map.html', measures=measures)


# Avvia la app Flask
if __name__ == '__main__':
    app.run(debug=True, host='0.0.0.0')
