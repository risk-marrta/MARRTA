# Generador de Flujograma desde Base de Datos
# ADAPTACIÓN PARA BASE DE DATOS: Lee datos desde MARRTA en lugar de Excel usando API Python
import sys
import re
from pathlib import Path
import pandas as pd
import numpy as np
import unicodedata as _u
import traceback

def safe_execute():
    """Wrapper to catch and display all errors with full traceback"""
    try:
        main_script()
    except Exception as e:
        print("")
        print("=" * 70)
        print("ERROR COMPLETO:")
        print("=" * 70)
        print(f"Tipo de error: {type(e).__name__}")
        print(f"Mensaje: {str(e)}")
        print("")
        print("Traceback completo:")
        traceback.print_exc()
        print("=" * 70)
        sys.stderr.flush()
        sys.stdout.flush()
        raise

def main_script():
    """Main script logic"""
    
    # --- Utilidades de texto y columnas ---
    def col(df, name):
        m = {c.lower(): c for c in df.columns}
        return m.get(name.lower())

    def col_like(df, pats):
        for p in pats:
            for c in df.columns:
                if re.search(p, c, flags=re.IGNORECASE):
                    return c
        return None

    def esc(s):
        s = "" if s is None else str(s)
        return s.replace("&","&amp;").replace("<","&lt;").replace(">","&gt;")

    def wrap(text, maxc=34):
        words = str(text).split()
        if not words: return [""]
        lines=[]; cur=""
        for w in words:
            if len(cur)+(1 if cur else 0)+len(w) <= maxc:
                cur = (cur+" "+w).strip()
            else:
                lines.append(cur); cur=w
        if cur: lines.append(cur)
        return lines

    def height_for(title, subtitle=""):
        n = len(wrap(title))+ (len(wrap(subtitle)) if subtitle else 0)
        return max(64, 18*n + 24*2)

    def normalize_txt(s):
        s = str(s).strip().lower()
        s = _u.normalize("NFKD", s)
        s = "".join(ch for ch in s if not _u.combining(ch))
        return s

    # --- Entradas/salidas ---
    # Note: __file__ not available in MARRTA's embedded Python console
    # Using current working directory instead
    try:
        proj_dir = Path(__file__).resolve().parent
    except NameError:
        import os
        proj_dir = Path(os.getcwd())
    
    # Generate output filename with date
    from datetime import datetime
    date_str = datetime.now().strftime("%Y-%m-%d")
    # Get model name from MARRTA configuration table
    try:
        config_result = Database.query("SELECT nombre FROM configuracion LIMIT 1")
        if config_result and len(config_result) > 0 and 'nombre' in config_result[0]:
            model_name = config_result[0]['nombre'] or "modelo"
        else:
            model_name = "modelo"
    except:
        model_name = "modelo"
    
    out_default = proj_dir / f"flujograma - {model_name} - {date_str}.html"

    # Permitir CLI: python generar_flujograma_v25_desde_bd.py [salida]
    if len(sys.argv) >= 2:
        out = Path(sys.argv[1])
    else:
        out = out_default

    print("=== Generador de Flujograma (desde Base de Datos) ===")
    print(f"Salida: {out}")
    print("")

    # --- Cargar datos desde MARRTA Python API ---
    print("Cargando datos desde la base de datos de MARRTA...")

    try:
        # Etapas
        etapas_data = Etapas.get_all()
        print(f"✓ Etapas: {len(etapas_data)} registros")
    
        # Sucesos (incluye etapa y subetapa ya mapeados)
        sucesos_data = Sucesos.get_all()
        print(f"✓ Sucesos: {len(sucesos_data)} registros")
    
        # Defensas (incluye tipo, etapa, robustez)
        defensas_data = Defensas.get_all()
        print(f"✓ Defensas: {len(defensas_data)} registros")
    
    except NameError as e:
        print(f"ERROR: {e}")
        print("")
        print("Este script debe ejecutarse desde el editor de Python de MARRTA")
        print("(Modelo → Scripting → Python), donde los módulos Etapas, Sucesos y Defensas están disponibles.")
        sys.exit(1)

    # Validar que hay datos
    if len(etapas_data) == 0:
        print("ERROR: No se encontraron etapas en la base de datos.")
        print("Asegúrate de que el modelo MARRTA tiene datos cargados.")
        sys.exit(1)

    if len(sucesos_data) == 0:
        print("ERROR: No se encontraron sucesos iniciadores en la base de datos.")
        print("Asegúrate de que el modelo MARRTA tiene sucesos iniciadores definidos.")
        sys.exit(1)

    # Convertir objetos Etapa a diccionarios
    # La API devuelve objetos Etapa con atributos (id, codigo, nombre, descripcion, orden)
    # en lugar de diccionarios, así que necesitamos convertirlos
    etapas_dict_list = []
    for etapa in etapas_data:
        etapas_dict_list.append({
            'id': etapa.id,
            'codigo': etapa.codigo,
            'nombre': etapa.nombre,
            'descripcion': etapa.descripcion,
            'orden': etapa.orden
        })

    # Convertir a DataFrames de pandas
    etapas = pd.DataFrame(etapas_dict_list)

    # Debug: Check sucesos_data structure
    print("")
    print("=== DEBUG: Estructura de sucesos_data ===")
    sys.stdout.flush()
    if len(sucesos_data) > 0:
        try:
            elem_type = type(sucesos_data[0])
            print(f"Tipo del primer elemento: {str(elem_type.__name__)}")
            sys.stdout.flush()
        
            if isinstance(sucesos_data[0], dict):
                print(f"Es diccionario - Claves: {list(sucesos_data[0].keys())}")
                sys.stdout.flush()
            else:
                print(f"Es objeto - Tipo: {elem_type.__name__}")
                sys.stdout.flush()
                try:
                    attrs = [attr for attr in dir(sucesos_data[0]) if not attr.startswith('_')]
                    print(f"Atributos disponibles: {attrs[:20]}")  # Limit to first 20
                    sys.stdout.flush()
                except:
                    print("No se pudieron obtener atributos")
                    sys.stdout.flush()
        except Exception as e:
            print(f"Error en debug de sucesos: {str(e)}")
            import traceback as tb
            tb.print_exc()
            sys.stdout.flush()
    print("")
    sys.stdout.flush()

    # Convert sucesos_data to dictionaries if needed (like we did for etapas)
    sucesos_dict_list = []
    for suceso in sucesos_data:
        if isinstance(suceso, dict):
            sucesos_dict_list.append(suceso)
        else:
            # Convert object to dictionary by extracting attributes
            suceso_dict = {}
            for attr in dir(suceso):
                if not attr.startswith('_'):
                    try:
                        suceso_dict[attr] = getattr(suceso, attr)
                    except Exception:
                        pass
            sucesos_dict_list.append(suceso_dict)

    # Same for defensas_data
    defensas_dict_list = []
    for defensa in defensas_data:
        if isinstance(defensa, dict):
            defensas_dict_list.append(defensa)
        else:
            # Convert object to dictionary by extracting attributes
            defensa_dict = {}
            for attr in dir(defensa):
                if not attr.startswith('_'):
                    try:
                        defensa_dict[attr] = getattr(defensa, attr)
                    except:
                        pass
            defensas_dict_list.append(defensa_dict)

    sucesos_raw = pd.DataFrame(sucesos_dict_list)
    defensas_raw = pd.DataFrame(defensas_dict_list)

    print(f"DEBUG: sucesos_raw columns: {list(sucesos_raw.columns)}")
    print(f"DEBUG: defensas_raw columns: {list(defensas_raw.columns)}")
    sys.stdout.flush()
    print("")

    # Validar que los DataFrames tienen las columnas esperadas
    required_etapas_cols = ["id", "codigo", "nombre"]
    missing_cols = [col for col in required_etapas_cols if col not in etapas.columns]
    if missing_cols:
        print(f"ERROR: DataFrame de etapas no contiene las columnas esperadas: {missing_cols}")
        print(f"Columnas disponibles: {list(etapas.columns)}")
        sys.exit(1)

    required_sucesos_cols = ["id", "codigo", "nombre", "etapa_id"]
    missing_cols = [col for col in required_sucesos_cols if col not in sucesos_raw.columns]
    if missing_cols:
        print(f"ERROR: DataFrame de sucesos no contiene las columnas esperadas: {missing_cols}")
        print(f"Columnas disponibles: {list(sucesos_raw.columns)}")
        sys.exit(1)

    print("")
    print("Procesando datos...")

    # --- Preparación de Etapas ---
    # Datos de Etapas desde BD: id, codigo, nombre, orden
    etapas["_Id"] = pd.to_numeric(etapas["id"], errors="coerce")
    etapas["_Nombre"] = etapas["nombre"].astype(str).str.strip()
    etapas["_Codigo"] = etapas["codigo"].astype(str).str.strip()
    etapas["_Tipo"] = np.where(etapas["_Id"]>0, "real", "ficticia")

    code2id = {str(c).strip(): int(i) for c, i in zip(etapas["_Codigo"], etapas["_Id"]) if pd.notna(i) and str(c).strip()}
    name2id = {str(n).strip(): int(i) for n, i in zip(etapas["_Nombre"], etapas["_Id"]) if pd.notna(i)}

    # --- Preparación de Subetapas ---
    # Desde sucesos_raw, extraer subetapas únicas (la BD devuelve subetapa_id, subetapa_nombre por suceso)
    subetapas_list = []
    seen_subs = set()
    for _, row in sucesos_raw.iterrows():
        sid = row.get("subetapa_id")
        snom = row.get("subetapa_nombre", "")
        etid = row.get("etapa_id")
        if pd.notna(sid) and sid not in seen_subs:
            seen_subs.add(sid)
            subetapas_list.append({"_Id": int(sid), "_Nombre": str(snom).strip(), "_EtapaId": int(etid) if pd.notna(etid) else None})

    subetapas = pd.DataFrame(subetapas_list) if subetapas_list else pd.DataFrame(columns=["_Id","_Nombre","_EtapaId"])

    # --- Preparación de Sucesos ---
    # Datos de sucesos desde BD: id, codigo, nombre, etapa_id, subetapa_id, ...
    sucesos = pd.DataFrame()
    sucesos["_Id"] = pd.to_numeric(sucesos_raw["id"], errors="coerce")
    sucesos["_Nombre"] = sucesos_raw["nombre"].astype(str).str.strip()
    sucesos["_Codigo"] = sucesos_raw["codigo"].astype(str).str.strip()
    sucesos["_EtapaId"] = pd.to_numeric(sucesos_raw["etapa_id"], errors="coerce")
    sucesos["_SubetapaId"] = pd.to_numeric(sucesos_raw.get("subetapa_id", pd.Series([None]*len(sucesos_raw))), errors="coerce")

    # --- Preparación de Defensas ---
    # La BD devuelve una fila por cada relación defensa-suceso
    # Necesitamos agrupar por (etapa_id, codigo) para crear defensas únicas con múltiples targets

    # Separar por tipo: 0=RF, 1=Barrera, 2=RC
    barreras_raw = defensas_raw[defensas_raw["tipo"] == 1].copy()
    rf_raw = defensas_raw[defensas_raw["tipo"] == 0].copy()
    rc_raw = defensas_raw[defensas_raw["tipo"] == 2].copy()

    def prep_defensas_from_db(df):
        """
        Convierte defensas de BD a formato agregado.
        BD tiene: id, codigo, nombre, tipo, robustez, etapa_orden_id, suceso_id
        Agregamos por (etapa_orden_id, codigo) y colectamos suceso_id en sets
        """
        if df.empty:
            return pd.DataFrame(columns=["_EtapaId","_Codigo","_Nombre","_Rob","_Targets"])
    
        d = pd.DataFrame()
        d["_Id"] = pd.to_numeric(df["id"], errors="coerce")
        d["_Nombre"] = df["nombre"].astype(str).str.strip()
        d["_Codigo"] = df["codigo"].astype(str).str.strip()
        # Nota: El nombre de columna 'etapa_orden_id' proviene de la respuesta de la API.
        # En otros DataFrames, como 'sucesos', se usa 'etapa_id'. Esta diferencia es intencional
        # para reflejar la estructura de los datos de origen y evitar confusiones al mapear campos.
        d["_EtapaId"] = pd.to_numeric(df["etapa_orden_id"], errors="coerce")
        d["_Rob"] = pd.to_numeric(df["robustez"], errors="coerce").fillna(0).clip(0,3).astype(int)
        d["_SucesoId"] = pd.to_numeric(df["suceso_id"], errors="coerce")
    
        # Agrupar por (EtapaId, Codigo) y agregar targets
        agg = d.groupby(["_EtapaId","_Codigo"], dropna=False).agg(
            _Nombre=("_Nombre","first"),
            _Rob=("_Rob","max"),
            _Targets=("_SucesoId", lambda ids: set(int(i) for i in ids if pd.notna(i)))
        ).reset_index()
    
        return agg

    barr_all = prep_defensas_from_db(barreras_raw)
    rf_all = prep_defensas_from_db(rf_raw)
    rc_all = prep_defensas_from_db(rc_raw)

    print(f"  - Barreras procesadas: {len(barr_all)} únicas")
    print(f"  - Reductores de Frecuencia: {len(rf_all)} únicos")
    print(f"  - Reductores de Consecuencia: {len(rc_all)} únicos")

    # Separar reales (Id>0) vs ficticias (Id<=0)
    agg_bar_real = barr_all[barr_all["_EtapaId"]>0].copy()
    agg_bar_fict = barr_all[barr_all["_EtapaId"]<=0].copy()
    agg_rf_real  = rf_all[rf_all["_EtapaId"]>0].copy()
    agg_rf_fict  = rf_all[rf_all["_EtapaId"]<=0].copy()
    agg_rc_real  = rc_all[rc_all["_EtapaId"]>0].copy()
    agg_rc_fict  = rc_all[rc_all["_EtapaId"]<=0].copy()

    print("")
    print("Generando layout...")


    # --- Parámetros de layout ---
    STAGE_HEADER_PAD = 100   # Vertical space reserved for stage title and border
    SUB_HEADER_PAD   = 60    # Vertical space reserved for sub-header (e.g., sub-stage title)
    BOTTOM_PAD       = 12    # Padding at the bottom of the layout

    node_w, node_h_min, v_gap = 420, 120, 50
    sub_w, sub_h_min, sub_gap = 380, 100, 14
    init_w_inside, init_h_min, init_gap = 340, 64, 10
    def_w, def_h_min, def_gap = 300, 60, 10

    # Posiciones X (con columna central en iniciadores)
    x_real = 1700
    x_bar_real = x_real - (def_w + 420)
    x_bar_fict = x_bar_real - (def_w + 420) - 200
    x_fict     = x_bar_fict - (node_w + 80)

    x_rf_real  = x_real + node_w + 260
    x_rc_real  = x_rf_real + (def_w + 160)
    x_rf_fict  = x_rc_real + (def_w + 260)
    x_rc_fict  = x_rf_fict + (def_w + 160)
    x_fict_R   = x_rc_fict + (node_w + 60)

    INIT_COL_CENTER = x_real - 240

    real = etapas[etapas["_Tipo"]=="real"].sort_values("_Id")
    ficts = etapas[etapas["_Tipo"]=="ficticia"].sort_values("_Id")

    subs_by_stage = {int(k):v.copy() for k,v in subetapas.groupby("_EtapaId") if pd.notna(k)}
    inits_by_sub = {int(k):v.copy() for k,v in sucesos.groupby("_SubetapaId") if pd.notna(k)}
    direct_inits_by_stage = {int(k):v.copy() for k,v in sucesos[sucesos["_SubetapaId"].isna()].groupby("_EtapaId") if pd.notna(k)}

    bar_by_stage_real = {int(k):v.copy() for k,v in agg_bar_real.groupby("_EtapaId") if pd.notna(k)}
    bar_by_stage_fict = {int(k):v.copy() for k,v in agg_bar_fict.groupby("_EtapaId") if pd.notna(k)}
    rf_by_stage_real  = {int(k):v.copy() for k,v in agg_rf_real.groupby("_EtapaId") if pd.notna(k)}
    rf_by_stage_fict  = {int(k):v.copy() for k,v in agg_rf_fict.groupby("_EtapaId") if pd.notna(k)}
    rc_by_stage_real  = {int(k):v.copy() for k,v in agg_rc_real.groupby("_EtapaId") if pd.notna(k)}
    rc_by_stage_fict  = {int(k):v.copy() for k,v in agg_rc_fict.groupby("_EtapaId") if pd.notna(k)}

    # --- Cálculo de alturas ---

    def compute_layout_h(etid):
        sblocks=[]; total_subs_h=0
        subs = subs_by_stage.get(etid, pd.DataFrame())
        if not subs.empty:
            for _, srow in subs.sort_values("_Id").iterrows():
                sid = int(srow["_Id"])
                inits = inits_by_sub.get(sid, pd.DataFrame())
                ihs = [max(init_h_min, height_for(r["_Nombre"], r.get("_Codigo","") or "")) for _,r in inits.iterrows()] if not inits.empty else []
                block_h = sum(ihs) + init_gap*(len(ihs)-1) if ihs else 0
                sh = max(sub_h_min, SUB_HEADER_PAD + block_h + BOTTOM_PAD)
                sblocks.append({"sid":sid,"row":srow,"h":sh,"inits":inits,"ihs":ihs,"block_h":block_h})
                total_subs_h += sh
            total_subs_h += sub_gap*(len(sblocks)-1)
        direct = direct_inits_by_stage.get(etid, pd.DataFrame())
        dih = [max(init_h_min, height_for(r["_Nombre"], r.get("_Codigo","") or "")) for _, r in direct.iterrows()] if not direct.empty else []
        direct_h = sum(dih) + init_gap*(len(dih)-1) if dih else 0
        stage_h_by_inits = max(node_h_min, STAGE_HEADER_PAD + total_subs_h + (sub_gap if (direct_h and total_subs_h) else 0) + direct_h + BOTTOM_PAD)

        def col_block_h(grp):
            if grp is None or grp.empty: return 0
            bhs=[max(def_h_min, height_for(r["_Nombre"], r["_Codigo"])) for _,r in grp.iterrows()]
            return sum(bhs) + def_gap*(len(bhs)-1)
        h_bar = col_block_h(bar_by_stage_real.get(etid))
        h_rf  = col_block_h(rf_by_stage_real.get(etid))
        h_rc  = col_block_h(rc_by_stage_real.get(etid))

        stage_h = max(stage_h_by_inits, h_bar, h_rf, h_rc, node_h_min)
        return {"subs":sblocks, "subs_total_h":total_subs_h, "stage_h":stage_h, "direct":direct, "dih":dih, "direct_h":direct_h}

    layout = {int(etid): compute_layout_h(int(etid)) for etid in real["_Id"].dropna().astype(int)}

    # Ficticias

    def fict_h(etid):
        def col_block_h(grp):
            if grp is None or grp.empty: return 0
            bhs=[max(def_h_min, height_for(r["_Nombre"], r["_Codigo"])) for _,r in grp.iterrows()]
            return sum(bhs) + def_gap*(len(bhs)-1)
        hb = col_block_h(bar_by_stage_fict.get(etid))
        hrf= col_block_h(rf_by_stage_fict.get(etid))
        hrc= col_block_h(rc_by_stage_fict.get(etid))
        return max(node_h_min, hb, hrf, hrc)

    fict_layout = {int(fr["_Id"]): fict_h(int(fr["_Id"])) for _, fr in ficts.iterrows()}

    # Posiciones Y
    stage_pos = {}; y = 0
    for etid in real["_Id"].dropna().astype(int):
        stage_pos[etid]=(y, layout[etid]["stage_h"])
        y += layout[etid]["stage_h"] + v_gap

    fict_pos_L = {}; yL = 0
    for _, fr in ficts.iterrows():
        etid=int(fr["_Id"]); H=fict_layout[etid]; fict_pos_L[etid]=(yL, H); yL += H + v_gap
    fict_pos_R = dict(fict_pos_L)

    # --- Render nodes/edges ---
    nodes=[]; edges=[]
    init_pos = {}
    def_pos = {}


    def add_node(x,y,w,h,cls,title,subtitle,tipo,etapa_id,code=None,extra=""):
        tl = wrap(title); sl = wrap(subtitle) if subtitle else []
        text = ['<text class="title" x="{cx}" y="{ty}">'.format(cx=w/2, ty=12)]
        for i,line in enumerate(tl):
            dy = ' dy="16"' if i else ''
            text.append('<tspan x="{cx}"{dy}>{t}</tspan>'.format(cx=w/2, dy=dy, t=esc(line)))
        if sl:
            text.append('<tspan x="{cx}" dy="24" class="subtitle">{t}</tspan>'.format(cx=w/2, t=esc(sl[0])))
            for line in sl[1:]:
                text.append('<tspan x="{cx}" dy="14" class="subtitle">{t}</tspan>'.format(cx=w/2, t=esc(line)))
        text.append("</text>")
        data_id = esc(str(code) if code is not None else "")
        nodes.append('<g class="node {cls}" transform="translate({x},{y})" data-name="{name}" data-code="{code}" data-tipo="{tipo}" data-etapa="{etapa}" {extra} tabindex="0">'
                     '<rect rx="12" ry="12" width="{w}" height="{h}"></rect>{text}</g>'.format(
                     cls=cls, x=x, y=y, name=esc(title), code=data_id, tipo=tipo, etapa=etapa_id, extra=extra, w=w, h=h, text="".join(text)))


    def add_edge_path(x1,y1,x2,y2,cls,df="",dt=""):
        edges.append(f'<path class="arrow {cls}" data-from="{esc(df)}" data-to="{esc(dt)}" marker-end="url(#arrowhead)" d="M {x1} {y1} L {x2} {y2}"></path>')

    # Reales (etapas, subetapas e iniciadores)
    for _, er in real.iterrows():
        etid=int(er["_Id"]); y0,H=stage_pos[etid]
        add_node(x_real, y0, node_w, H, "real", er["_Nombre"], er.get("_Codigo","") or "", "Etapa", etid)

        sub_y=y0 + STAGE_HEADER_PAD
        for sb in layout[etid]["subs"]:
            sid, sh = sb["sid"], sb["h"]
            sx = x_real + (node_w - sub_w)/2
            add_node(sx, sub_y, sub_w, sh, "subetapa", sb["row"]["_Nombre"], "", "Subetapa", etid, extra=f'data-subetapa="{sid}"')
            inits=sb["inits"]; ihs=sb["ihs"]; block_h=sb["block_h"]
            if not inits.empty:
                avail = sh - SUB_HEADER_PAD - BOTTOM_PAD
                ix = sx + (sub_w - init_w_inside)/2
                inner_offset = max(0, (avail - block_h)/2)
                iy = sub_y + SUB_HEADER_PAD + inner_offset
                for j,(_,ir) in enumerate(inits.sort_values("_Id").iterrows()):
                    ih=ihs[j]; iidv=int(ir["_Id"])
                    add_node(ix, iy, init_w_inside, ih, "iniciador", ir["_Nombre"], ir.get("_Codigo","") or "", "Iniciador", etid, code=iidv, extra=f'data-subetapa="{sid}"')
                    init_pos[iidv]=(ix, iy, init_w_inside, ih)
                    iy += ih + init_gap
            sub_y += sh + sub_gap

        direct=layout[etid]["direct"]; dih=layout[etid]["dih"]; direct_h=layout[etid]["direct_h"]; subs_total=layout[etid]["subs_total_h"]
        if not direct.empty:
            if subs_total>0:
                start_y = y0 + STAGE_HEADER_PAD + subs_total + (sub_gap if direct_h else 0)
            else:
                start_y = y0 + STAGE_HEADER_PAD
            ix = x_real + (node_w - init_w_inside)/2
            iy = start_y + max(0, (direct_h - sum(dih) - init_gap*(len(dih)-1))/2) if len(dih)>0 else start_y
            for j,(_,ir) in enumerate(direct.sort_values("_Id").iterrows()):
                ih=dih[j]; iidv=int(ir["_Id"])
                add_node(ix, iy, init_w_inside, ih, "iniciador", ir["_Nombre"], ir.get("_Codigo","") or "", "Iniciador", etid, code=iidv)
                init_pos[iidv]=(ix, iy, init_w_inside, ih)
                iy += ih + init_gap

    # Barreras reales izquierda
    for etid, grp in bar_by_stage_real.items():
        if etid not in stage_pos: continue
        y0,H = stage_pos[etid]
        bhs=[max(def_h_min, height_for(r["_Nombre"], r["_Codigo"])) for _,r in grp.iterrows()]
        block_h=sum(bhs)+def_gap*(len(bhs)-1) if bhs else 0
        by=y0+(H-block_h)/2
        for j, (_, br) in enumerate(grp.sort_values("_Codigo").iterrows()):
            bh=bhs[j]; code = br["_Codigo"]; rob=int(br["_Rob"]); name = br["_Nombre"]
            key=f"bar|{etid}|{code}"
            add_node(x_bar_real, by, def_w, bh, "barrera", name, code or "", "Barrera", etid, code=code, extra=f'data-rob="{rob}" data-key="{key}"')
            def_pos[key]=(x_bar_real, by, def_w, bh)
            by += bh + def_gap

    # RF/RC reales derecha
    for (colx, grp_by_stage, cls, tipo, prefix) in [
        (x_rf_real, rf_by_stage_real, "rf", "Reductor de Frecuencia", "rf"),
        (x_rc_real, rc_by_stage_real, "rc", "Reductor de Consecuencias", "rc")
    ]:
        for etid, grp in grp_by_stage.items():
            if etid not in stage_pos: continue
            y0,H = stage_pos[etid]
            bhs=[max(def_h_min, height_for(r["_Nombre"], r["_Codigo"])) for _,r in grp.iterrows()] if not grp.empty else []
            block_h=sum(bhs)+def_gap*(len(bhs)-1) if bhs else 0
            by=y0+(H-block_h)/2
            for j, (_, dr) in enumerate(grp.sort_values("_Codigo").iterrows()):
                bh=bhs[j]; code = dr["_Codigo"]; rob=int(dr["_Rob"]); name = dr["_Nombre"]
                key=f"{prefix}|{etid}|{code}"
                add_node(colx, by, def_w, bh, cls, name, code or "", tipo, etid, code=code, extra=f'data-rob="{rob}" data-key="{key}"')
                def_pos[key]=(colx, by, def_w, bh)
                by += bh + def_gap

    # Ficticias izquierda + barreras
    for _, fr in ficts.iterrows():
        etid=int(fr["_Id"]); y0,H=fict_pos_L[etid]
        add_node(x_fict, y0, node_w, H, "ficticia", fr["_Nombre"], fr.get("_Codigo","") or "", "Etapa Ficticia", etid)
        grp = bar_by_stage_fict.get(etid, pd.DataFrame())
        if not grp.empty:
            bhs=[max(def_h_min, height_for(r["_Nombre"], r["_Codigo"])) for _,r in grp.iterrows()]
            block_h=sum(bhs)+def_gap*(len(bhs)-1)
            by=y0+(H-block_h)/2
            for j, (_, br) in enumerate(grp.sort_values("_Codigo").iterrows()):
                bh=bhs[j]; code = br["_Codigo"]; rob=int(br["_Rob"]); name = br["_Nombre"]
                key=f"bar|{etid}|{code}"
                add_node(x_bar_fict, by, def_w, bh, "barrera", name, code or "", "Barrera", etid, code=code, extra=f'data-rob="{rob}" data-key="{key}"')
                def_pos[key]=(x_bar_fict, by, def_w, bh)
                by += bh + def_gap

    # RF/RC ficticias (derecha extendida)
    for (colx, grp_by_stage, cls, tipo, prefix) in [
        (x_rf_fict, rf_by_stage_fict, "rf", "Reductor de Frecuencia", "rf"),
        (x_rc_fict, rc_by_stage_fict, "rc", "Reductor de Consecuencias", "rc")
    ]:
        for etid, grp in grp_by_stage.items():
            if etid not in fict_pos_L: continue
            y0,H = fict_pos_L[etid]
            bhs=[max(def_h_min, height_for(r["_Nombre"], r["_Codigo"])) for _,r in grp.iterrows()] if not grp.empty else []
            block_h=sum(bhs)+def_gap*(len(bhs)-1) if bhs else 0
            by=y0+(H-block_h)/2
            for j, (_, dr) in enumerate(grp.sort_values("_Codigo").iterrows()):
                bh=bhs[j]; code = dr["_Codigo"]; rob=int(dr["_Rob"]); name = dr["_Nombre"]
                key=f"{prefix}|{etid}|{code}"
                add_node(colx, by, def_w, bh, cls, name, code or "", tipo, etid, code=code, extra=f'data-rob="{rob}" data-key="{key}"')
                def_pos[key]=(colx, by, def_w, bh)
                by += bh + def_gap

    # Ficticias espejo derecha
    for _, fr in ficts.iterrows():
        etid=int(fr["_Id"]); y0,H=fict_pos_R[etid]
        add_node(x_fict_R, y0, node_w, H, "ficticia", fr["_Nombre"], fr.get("_Codigo","") or "", "Etapa Ficticia", etid)

    # Flechas defensas→iniciadores (paths rectos)

    def add_edges_from_agg(agg_df, prefix):
        for _, row in agg_df.iterrows():
            etid = int(row["_EtapaId"]); code = row["_Codigo"]; key = f"{prefix}|{etid}|{code}"
            if key not in def_pos: continue
            bx, by, bw, bh = def_pos[key]; cy = by + bh/2
            for iidv in sorted(int(i) for i in row["_Targets"] if int(i) in init_pos.keys()):
                ix, iy, iw, ih = init_pos[iidv]
                cls = "arrow-bar" if prefix=="bar" else ("arrow-rf" if prefix=="rf" else "arrow-rc")
                # Conexión: Barreras (izquierda) → al borde izquierdo del iniciador; RF/RC (derecha) → al borde derecho del iniciador
                if prefix in ("rf","rc"):
                    x1 = bx                 # borde izquierdo de la defensa (a la derecha del canvas)
                    x2 = ix + iw            # borde derecho del iniciador
                else:
                    x1 = bx + bw            # borde derecho de la defensa (a la izquierda del canvas)
                    x2 = ix                 # borde izquierdo del iniciador
                edges.append(f'<path class="arrow {cls}" data-from="{esc(prefix+":"+str(etid)+"|"+str(code))}" data-to="init:{iidv}" marker-end="url(#arrowhead)" d="M {x1} {cy} L {x2} {iy+ih/2}"></path>')

    add_edges_from_agg(agg_bar_real, "bar")
    add_edges_from_agg(agg_bar_fict, "bar")
    add_edges_from_agg(agg_rf_real, "rf")
    add_edges_from_agg(agg_rc_real, "rc")
    add_edges_from_agg(agg_rf_fict, "rf")
    add_edges_from_agg(agg_rc_fict, "rc")

    # Cabeceras
    headers = []

    def add_header(x, text):
        headers.append(f'<text class="colhdr" x="{x}" y="-80">{esc(text)}</text>')

    add_header(x_bar_fict + def_w/2, "Barreras")
    add_header(x_bar_real + def_w/2, "Barreras")
    add_header(x_real + node_w/2, "Etapas / Subetapas / Iniciadores")
    add_header(x_rf_real + def_w/2, "Reductores Frecuencia")
    add_header(x_rc_real + def_w/2, "Reductores Consecuencias")
    add_header(x_rf_fict + def_w/2, "Reductores Frecuencia")
    add_header(x_rc_fict + def_w/2, "Reductores Consecuencias")
    add_header(x_fict + node_w/2, "Etapas ficticias")
    add_header(x_fict_R + node_w/2, "Etapas ficticias")

    leftmost = min(x_fict, x_bar_fict) - 200
    rightmost = x_fict_R + node_w + 200
    width = int(rightmost - leftmost)
    height = int(max(max(v[0]+v[1] for v in stage_pos.values()) if stage_pos else 0,
                     max(v[0]+v[1] for v in fict_pos_L.values()) if fict_pos_L else 0) + 720)

    # Opciones de etapas para selects
    # - Iniciadores: solo etapas reales (Id>0), orden ascendente
    opt_inits = ''.join(
        f'<option value="{int(i)}">{esc(n)}</option>'
        for i,n in zip(etapas[etapas["_Id"]>0].sort_values("_Id")["_Id"], etapas[etapas["_Id"]>0].sort_values("_Id")["_Nombre"]) if pd.notna(i)
    )
    # - Defensas: todas las etapas, orden ascendente
    opt_defs = ''.join(
        f'<option value="{int(i)}">{esc(n)}</option>'
        for i,n in zip(etapas.sort_values("_Id")["_Id"], etapas.sort_values("_Id")["_Nombre"]) if pd.notna(i)
    )

    print("Layout completo. Generando HTML...")


    # --- Plantilla HTML ---
    # Using raw string to avoid SyntaxWarning for escape sequences
    template = r"""<!DOCTYPE html>
    <html lang="es"><head><meta charset="utf-8"><meta name="viewport" content="width=device-width, initial-scale=1">
    <title>Flujograma vertical (desde BD)</title>
    <style>
    :root{--card:#fff;--bd:#e5e7eb;--txt:#0f172a;--muted:#475569;--bg:#f4f6fb}
    body{margin:0;font-family:system-ui,-apple-system,Segoe UI,Roboto,Arial,sans-serif;background:var(--bg)}
    #wrap{height:100vh;width:100vw;overflow:hidden}
    svg{width:100%;height:100%;display:block;cursor:grab}svg:active{cursor:grabbing}
    .node rect{fill:#fff;stroke:var(--txt);stroke-width:1.2}
    .node.real rect{fill:#dbeafe}.node.subetapa rect{fill:#eff6ff;stroke-dasharray:3 2}.node.iniciador rect{fill:#fff7ed}
    .node.ficticia rect{fill:#e5e7eb}
    /* Paleta por robustez (Barreras, RF, RC) */
    .node.barrera rect,.node.rf rect,.node.rc rect{fill:#ffffff}
    .node.barrera[data-rob="1"] rect,.node.rf[data-rob="1"] rect,.node.rc[data-rob="1"] rect{fill:#eefdd3}
    .node.barrera[data-rob="2"] rect,.node.rf[data-rob="2"] rect,.node.rc[data-rob="2"] rect{fill:#ffedd5}
    .node.barrera[data-rob="3"] rect,.node.rf[data-rob="3"] rect,.node.rc[data-rob="3"] rect{fill:#fee2e2}
    .node .title{font-size:16px;font-weight:600;text-anchor:middle;dominant-baseline:hanging;fill:var(--txt)}
    .node .subtitle{font-size:16px;fill:#334155;text-anchor:middle;dominant-baseline:hanging}
    .node:focus rect,.node:hover rect{stroke-width:2}
    .arrow{stroke:var(--txt);stroke-width:1;opacity:.5;fill:none}
    .arrow-rf{stroke:#16a34a}
    .arrow-rc{stroke:#2563eb}
    .arrow.focus{opacity:1; stroke-width:2.6}
    /* Atenuación específica para flechas (más fuerte que .dim/.dimx genérico) */
    .arrow.dim{opacity:.20}
    .arrow.dimx{opacity:.08}
    .dim{opacity:.25} .dimx{opacity:.12} .hi{outline:2px solid #2563eb}
    .colhdr{font-weight:600;fill:#111827;text-anchor:middle;font-size:18px}
    .panel{position:fixed;right:12px;top:12px;background:var(--card);border:1px solid var(--bd);border-radius:12px;box-shadow:0 2px 12px rgba(0,0,0,.08);z-index:5;max-width:520px;min-width:360px}
    .panel .handle{cursor:move;background:#f8fafc;border-bottom:1px solid var(--bd);padding:7.5px 12px;border-top-left-radius:12px;border-top-right-radius:12px;font-weight:525;display:flex;justify-content:space-between;align-items:center}
    .panel .content{padding:10px 12px;display:grid;gap:10px}
    .panel.collapsed .content{display:none}
    .card{border:1px solid var(--bd);border-radius:10px;padding:8px 10px}
    .card h4{margin:0 0 6px 0;font-size:13px;color:var(--muted)}
    .grid2{display:grid;grid-template-columns:1fr 1fr;gap:8px}
    .grid3{display:grid;grid-template-columns:1fr 1fr 1fr;gap:8px}
    .row{display:flex;gap:8px;align-items:center;flex-wrap:wrap}
    label.small{font-size:12px;color:#374151}
    select,input[type="text"]{width:100%;padding:6px 8px;border:1px solid var(--bd);border-radius:8px;font-size:13px}
    button.btn{padding:6px 10px;border-radius:8px;border:1px solid var(--bd);background:#f9fafb;cursor:pointer}
    .badge{display:inline-block;padding:2px 6px;border-radius:6px;border:1px solid var(--bd);font-size:11px;color:var(--muted)}
    .footer{display:flex;justify-content:space-between;align-items:center;gap:8px;flex-wrap:wrap}
    .counts{font-size:12px;color:#374151}
    .panel.bottom{left:50%;transform:translateX(-50%);right:auto;top:auto;bottom:12px}
    </style></head>
    <body>
    <div id="wrap">
      <svg id="canvas" data-initcol="[[INITCOL]]" xmlns="http://www.w3.org/2000/svg" viewBox="0 0 [[WIDTH]] [[HEIGHT]]" role="img" aria-label="flujograma">
        <defs><marker id="arrowhead" markerWidth="10" markerHeight="7" refX="8" refY="3.5" orient="auto"><polygon points="0 0, 10 3.5, 0 7" fill="#0f172a"></polygon></marker></defs>
        <g id="viewport" transform="translate(240,160) scale(1)">
          <g id="edges">[[EDGES]]</g>
          <g id="nodes">[[NODES]]</g>
          <g id="headers">[[HEADERS]]</g>
        </g>
      </svg>

      <div id="panel" class="panel">
        <div class="handle">
          <span>[[MODEL_NAME]]</span>
          <button id="toggle-panel" class="btn">▾</button>
        </div>
        <div class="content">
          <div class="card">
            <h4>Filtrar por etapas</h4>
            <div class="grid2">
              <div><label class="small">Etapa de <b>sucesos</b></label><select id="f-etapa-inits">
                <option value="">(todas)</option>[[OPT_INITS]]
              </select></div>
              <div><label class="small">Etapa de <b>defensas</b></label><select id="f-etapa-defs">
                <option value="">(todas)</option>[[OPT_DEFS]]
              </select></div>
            </div>
          </div>

          <div class="card">
            <h4>Mostrar</h4>
            <div class="row">
              <label class="small"><input type="checkbox" id="show-bar" checked> Barreras</label>
              <label class="small"><input type="checkbox" id="show-rf" checked> Reductores Frecuencia</label>
              <label class="small"><input type="checkbox" id="show-rc" checked> Reductores Consecuencias</label>
            </div>
            <div class="row">
              <label class="small"><input type="checkbox" id="only-defs-on-visible"> Solo defensas que actúan sobre sucesos visibles</label>
            </div>
          </div>

          <div class="card">
            <h4>Búsqueda</h4>
            <div>
              <input type="text" id="f-texto" placeholder="Nombre o código…" style="width:100%">
            </div>
          </div>

          <div class="card">
            <h4>Robustez (todas las defensas)</h4>
            <div class="row" id="robbox">
              <label class="small"><input type="checkbox" class="rob" value="0" checked> 0 baja</label>
              <label class="small"><input type="checkbox" class="rob" value="1" checked> 1 normal</label>
              <label class="small"><input type="checkbox" class="rob" value="2" checked> 2 robusta</label>
              <label class="small"><input type="checkbox" class="rob" value="3" checked> 3 muy robusta</label>
            </div>
          </div>

          <div class="footer">
            <div class="row">
              <button id="zoom-in" class="btn">+</button>
              <button id="zoom-out" class="btn">−</button>
              <button id="reset" class="btn">Reiniciar</button>
            </div>
            <div class="row">
              <small style="color:#64748b">💡 Zoom: rueda ratón | Pan: arrastra | Pan rápido: Shift+arrastra</small>
            </div>
            <div class="row"><span id="js-status" class="badge">JS listo</span></div>
          </div>

          <div class="counts">Iniciadores visibles: <b id="count-inits">–</b> · RF visibles: <b id="count-rf">–</b> · Barreras visibles: <b id="count-bar">–</b> · RC visibles: <b id="count-rc">–</b></div>
        </div>
      </div>
      <script>
    (function(){
      const svg = document.getElementById('canvas');
      const vp  = document.getElementById('viewport');
      const statusEl = document.getElementById('js-status');
      function setStatus(t){ if(statusEl) statusEl.textContent = t; }

      let INIT_COL_X = parseFloat(svg.getAttribute('data-initcol')) || 0;
      let scale = 1, minScale = 0.12, maxScale = 18, tx = 240, ty = 160;
      let dragging=false, lx=0, ly=0;
      const PAN_K = 8.0, TURBO = 2.0;
      function state(){ return `scale=${scale.toFixed(2)} tx=${tx.toFixed(0)} ty=${ty.toFixed(0)} initx=${INIT_COL_X.toFixed(0)}`; }
      function update(){ vp.setAttribute('transform', 'translate(' + tx + ',' + ty + ') scale(' + scale + ')'); setStatus(state()); }

      function zoomAroundInitCol(delta, anchorY=null){
        const ns = Math.min(maxScale, Math.max(minScale, scale + delta));
        if(ns===scale) return;
        const screenX_init = tx + scale * INIT_COL_X;
        tx = screenX_init - ns * INIT_COL_X;
        const rect = svg.getBoundingClientRect();
        const screenY = (anchorY==null) ? (rect.top + rect.height/2) : anchorY;
        const worldY = (screenY - ty) / scale;
        ty = screenY - ns * worldY;
        scale = ns; update();
      }

      svg.addEventListener('wheel', function(e){ e.preventDefault(); const d = -Math.sign(e.deltaY)*0.22; zoomAroundInitCol(d, e.clientY); }, {passive:false});
      document.getElementById('zoom-in').addEventListener('click', function(){ zoomAroundInitCol(+0.35); });
      document.getElementById('zoom-out').addEventListener('click', function(){ zoomAroundInitCol(-0.35); });
      document.getElementById('reset').addEventListener('click', function(){ scale=1; tx=240; ty=160; update(); });

      svg.addEventListener('mousedown', function(e){ dragging=true; lx=e.clientX; ly=e.clientY; });
      window.addEventListener('mousemove', function(e){ if(!dragging) return; const turbo = e.shiftKey ? TURBO : 1.0; const dx=(e.clientX-lx)*PAN_K*turbo, dy=(e.clientY-ly)*PAN_K*turbo; lx=e.clientX; ly=e.clientY; tx+=dx; ty+=dy; update(); });
      window.addEventListener('mouseup', function(){ dragging=false; });

      const fEtapaInits=document.getElementById('f-etapa-inits'),
            fEtapaDefs =document.getElementById('f-etapa-defs'),
            fTexto     =document.getElementById('f-texto'),
            onlyDefsOnVisible =document.getElementById('only-defs-on-visible'),
            robChecks  = Array.from(document.querySelectorAll('.rob')),
            showBar    = document.getElementById('show-bar'),
            showRF     = document.getElementById('show-rf'),
            showRC     = document.getElementById('show-rc');

      function isVisible(el){ return el && el.style.display !== 'none'; }
      function nodeByKey(key){
        const [type, rest]=(key||'').split(':');
        if(type==='init'){ return document.querySelector('.node.iniciador[data-code="'+rest+'"]'); }
        if(type==='bar' || type==='rf' || type==='rc'){
          return document.querySelector('.node[data-key="'+type+'|'+rest+'"]');
        }
        return null;
      }
      function getVisibleInitiators(){
        const ids = new Set();
        document.querySelectorAll('.node.iniciador').forEach(function(el){ if(isVisible(el)){ ids.add(el.getAttribute('data-code')); } });
        return ids;
      }
      function updateCounters(){
        const nInits = Array.from(document.querySelectorAll('.node.iniciador')).filter(isVisible).length;
        const nRF  = Array.from(document.querySelectorAll('.node.rf')).filter(isVisible).length;
        const nBar = Array.from(document.querySelectorAll('.node.barrera')).filter(isVisible).length;
        const nRC  = Array.from(document.querySelectorAll('.node.rc')).filter(isVisible).length;
        const ci=document.getElementById('count-inits'), crf=document.getElementById('count-rf');
        const cbar=document.getElementById('count-bar'), crc=document.getElementById('count-rc');
        if(ci) ci.textContent = nInits; 
        if(crf) crf.textContent = nRF;
        if(cbar) cbar.textContent = nBar;
        if(crc) crc.textContent = nRC;
      }

      function showEdgeIfNodesVisible(l){
        const kf=l.getAttribute('data-from')||''; const kt=l.getAttribute('data-to')||'';
        const nf=nodeByKey(kf); const nt=nodeByKey(kt);
        const visNodes = (!kf||(nf&&isVisible(nf))) && (!kt||(nt&&isVisible(nt)));
        let visType = true;
        if(l.classList.contains('arrow-bar') && !showBar.checked) visType=false;
        if(l.classList.contains('arrow-rf')  && !showRF.checked)  visType=false;
        if(l.classList.contains('arrow-rc')  && !showRC.checked)  visType=false;
        l.style.display = (visNodes && visType) ? '' : 'none';
      }

      function apply(){
        const etInits=fEtapaInits.value; 
        const etDefs =fEtapaDefs.value; 
        const q=(fTexto.value||'').toLowerCase();
        const robSelected = new Set(robChecks.filter(c=>c.checked).map(c=>c.value));
        const onlyOnVisible = onlyDefsOnVisible.checked;

        document.querySelectorAll('.node').forEach(function(el){
          const tipo=el.getAttribute('data-tipo')||'';
          const name=(el.getAttribute('data-name')||'').toLowerCase();
          const code=(el.getAttribute('data-code')||'').toLowerCase();
          const eid =el.getAttribute('data-etapa')||'';
          let vis=true;

          // Subetapas and Iniciadores are always shown (checkboxes removed)

          if(tipo==='Barrera' || tipo==='Reductor de Frecuencia' || tipo==='Reductor de Consecuencias'){
            const rob = el.getAttribute('data-rob')||'';
            if(!robSelected.has(rob)) vis=false;
            if(etDefs && eid && eid!=etDefs) vis=false;
            if(tipo==='Barrera' && !showBar.checked) vis=false;
            if(tipo==='Reductor de Frecuencia' && !showRF.checked) vis=false;
            if(tipo==='Reductor de Consecuencias' && !showRC.checked) vis=false;
          }

          if((tipo==='Iniciador' || tipo==='Subetapa' || tipo==='Etapa' || tipo==='Etapa Ficticia')){
            if(etInits && eid && eid!=etInits) vis=false;
          }

          if(q && !(name.includes(q)||code.includes(q))) vis=false;
          el.style.display = vis ? '' : 'none';
        });

        if(onlyOnVisible){
          const visibleInits = getVisibleInitiators();
          const involvedDefs = new Set();
          document.querySelectorAll('.arrow').forEach(function(edge){
            const to=edge.getAttribute('data-to')||'';
            const from=edge.getAttribute('data-from')||'';
            const partsTo=to.split(':'), partsFrom=from.split(':');
            if(partsTo[0]==='init' && visibleInits.has(partsTo[1]) && (partsFrom[0]==='bar' || partsFrom[0]==='rf' || partsFrom[0]==='rc')){
              involvedDefs.add(partsFrom[0]+':'+partsFrom[1]);
            }
          });
          document.querySelectorAll('.node.barrera, .node.rf, .node.rc').forEach(function(b){
            const type = b.classList.contains('barrera')?'bar':(b.classList.contains('rf')?'rf':'rc');
            const key = b.getAttribute('data-key')||'';
            const keyShort = key.split('|').slice(1).join('|'); // EtapaId|Codigo
            if(b.style.display !== 'none' && !involvedDefs.has(type+':'+keyShort)){
              b.style.display='none';
            }
          });
        }

        document.querySelectorAll('.arrow').forEach(showEdgeIfNodesVisible);
        updateCounters();
      }

      function clearHighlights(){
        document.querySelectorAll('.node.barrera, .node.rf, .node.rc, .node.iniciador').forEach(el=>{ el.classList.remove('dim','dimx','hi'); });
        document.querySelectorAll('.arrow').forEach(el=>{ el.classList.remove('dim','dimx','focus'); });
      }

      // Selección extendida: Barreras, RF y RC participan
      function bindInteractions(){
        // Clic en iniciador → resalta defensas (bar/rf/rc) que conectan con ese iniciador y solo sus flechas hacia ese iniciador; lo demás se difumina
        document.querySelectorAll('.node.iniciador').forEach(function(init){
          init.addEventListener('click', function(e){
            e.stopPropagation();
            const iid = init.getAttribute('data-code');
            const involved = new Set(); // combos completos: bar:key, rf:key, rc:key
            document.querySelectorAll('.arrow[data-to="init:'+iid+'"]').forEach(function(edge){
              const kf=edge.getAttribute('data-from')||'';
              const parts=kf.split(':');
              if(parts[0]==='bar' || parts[0]==='rf' || parts[0]==='rc') involved.add(kf);
            });
            clearHighlights();
            // Defensas: barreras, rf y rc
            document.querySelectorAll('.node.barrera, .node.rf, .node.rc').forEach(function(dn){
              const type = dn.classList.contains('barrera')?'bar':(dn.classList.contains('rf')?'rf':'rc');
              const key  = dn.getAttribute('data-key');
              const keyShort = key.split('|').slice(1).join('|'); // EtapaId|Codigo
              const combo = type+':'+keyShort; // forma como en data-from
              if(!involved.has(combo)){ dn.classList.add('dim'); } else { dn.classList.add('hi'); }
            });
            // Flechas: marcar .focus las que terminan en ese iniciador y parten de una defensa involucrada; las demás .dim
            document.querySelectorAll('.arrow').forEach(function(edge){
              const from=edge.getAttribute('data-from')||'';
              const to=edge.getAttribute('data-to')||'';
              if(to==='init:'+iid && involved.has(from)) edge.classList.add('focus');
              else edge.classList.add('dim');
            });
          });
        });
        // Clic en defensa (bar/rf/rc) → seleccionar esa defensa; dimx otras defensas, dimx flechas no salientes de esa defensa, dimx iniciadores no conectados a esa defensa
        document.querySelectorAll('.node.barrera, .node.rf, .node.rc').forEach(function(defNode){
          defNode.addEventListener('click', function(e){
            e.stopPropagation();
            const type = defNode.classList.contains('barrera')?'bar':(defNode.classList.contains('rf')?'rf':'rc');
            const key  = defNode.getAttribute('data-key');
            const keyShort = key.split('|').slice(1).join('|');
            const combo = type+':'+keyShort;
            // Obtener iniciadores conectados a esta defensa
            const connectedInits = new Set();
            document.querySelectorAll('.arrow').forEach(function(edge){
              const from=edge.getAttribute('data-from')||'';
              const to=edge.getAttribute('data-to')||'';
              if(from===combo){
                const parts=to.split(':'); if(parts[0]==='init' && parts[1]) connectedInits.add(parts[1]);
              }
            });
            clearHighlights();
            // Defensas: solo esta queda nítida
            document.querySelectorAll('.node.barrera, .node.rf, .node.rc').forEach(function(dn){
              const t = dn.classList.contains('barrera')?'bar':(dn.classList.contains('rf')?'rf':'rc');
              const k = dn.getAttribute('data-key');
              const kShort = k.split('|').slice(1).join('|');
              if(t+':'+kShort!==combo){ dn.classList.add('dimx'); } else { dn.classList.add('hi'); }
            });
            // Flechas: las que salen de esta defensa → .focus; el resto .dimx
            document.querySelectorAll('.arrow').forEach(function(edge){
              const from=edge.getAttribute('data-from')||'';
              if(from===combo) edge.classList.add('focus');
              else edge.classList.add('dimx');
            });
            // Iniciadores: dimx los no conectados
            document.querySelectorAll('.node.iniciador').forEach(function(init){
              const iid = init.getAttribute('data-code');
              if(!connectedInits.has(iid)) init.classList.add('dimx');
            });
          });
        });
        // Click en fondo limpia selección
        document.getElementById('wrap').addEventListener('click', function(){ clearHighlights(); });
      }

      // Tooltip sencillo
      const tooltip = (function(){
        const el = document.createElement('div'); el.id='tooltip'; el.style.position='fixed'; el.style.pointerEvents='none';
        el.style.background='rgba(15,23,42,.92)'; el.style.color='#fff'; el.style.padding='8px 10px'; el.style.borderRadius='8px'; el.style.fontSize='12px';
        el.style.lineHeight='1.35'; el.style.transform='translate(-50%,-120%)'; el.style.opacity='0'; el.style.transition='opacity .15s'; document.body.appendChild(el); return el;
      })();
      function showTip(evt, el){
        const name=el.getAttribute('data-name')||'', code=el.getAttribute('data-code')||'', tipo=el.getAttribute('data-tipo')||'', etapa=el.getAttribute('data-etapa')||'';
        const rob=el.getAttribute('data-rob'); 
        const robtxt = (rob==='0')?'baja':(rob==='1')?'normal':(rob==='2')?'robusta':(rob==='3')?'muy robusta':'';
        tooltip.innerHTML = '<strong>'+name+'</strong><br/>Tipo: '+tipo+' · Etapa: '+etapa + (code?'<br/>Código: '+code:'') + (robtxt?('<br/>Robustez: '+robtxt):'');
        tooltip.style.left = evt.clientX+'px'; tooltip.style.top = evt.clientY+'px'; tooltip.style.opacity='1';
      }
      function hideTip(){ tooltip.style.opacity='0'; }
      document.querySelectorAll('.node').forEach(function(el){
        el.addEventListener('mousemove', function(e){ showTip(e, el); });
        el.addEventListener('mouseleave', hideTip);
      });

      const controls = [document.getElementById('f-etapa-inits'), document.getElementById('f-etapa-defs'), document.getElementById('f-texto'),
                        document.getElementById('only-defs-on-visible'),
                        document.getElementById('show-bar'), document.getElementById('show-rf'), document.getElementById('show-rc')]
                        .concat(Array.from(document.querySelectorAll('.rob')));
      controls.forEach(c=>{ c.addEventListener('input', function(){ apply(); document.querySelectorAll('.arrow').forEach(showEdgeIfNodesVisible); }); 
                           c.addEventListener('change', function(){ apply(); document.querySelectorAll('.arrow').forEach(showEdgeIfNodesVisible); }); });

      // Panel: arrastrable y colapsable
      const panelEl = document.getElementById('panel');
      const handleEl = panelEl.querySelector('.handle');
      let draggingPanel=false, px0=0, py0=0, pLeft=0, pTop=0;
      handleEl.addEventListener('mousedown', function(e){
        if((e.target && e.target.id)==='toggle-panel') return; // no iniciar drag al pulsar el botón
        draggingPanel=true; const rect=panelEl.getBoundingClientRect();
        pLeft = rect.left; pTop = rect.top; px0=e.clientX; py0=e.clientY;
        panelEl.style.right='auto'; panelEl.style.bottom='auto';
        document.body.style.userSelect='none';
      });
      window.addEventListener('mousemove', function(e){
        if(!draggingPanel) return;
        const nx = pLeft + (e.clientX - px0);
        const ny = pTop  + (e.clientY - py0);
        panelEl.style.left = nx+'px';
        panelEl.style.top  = ny+'px';
      });
      window.addEventListener('mouseup', function(){ draggingPanel=false; document.body.style.userSelect=''; });

      const toggleBtn = document.getElementById('toggle-panel');
      if(toggleBtn){ toggleBtn.addEventListener('click', function(e){ e.stopPropagation(); panelEl.classList.toggle('collapsed'); }); }

      // Export helpers
      function download(filename, dataBlob){
        const a = document.createElement('a');
        a.href = URL.createObjectURL(dataBlob);
        a.download = filename;
        document.body.appendChild(a);
        a.click();
        setTimeout(()=>{ URL.revokeObjectURL(a.href); document.body.removeChild(a); }, 0);
      }
      function cloneWithViewportTransform(){
        const svgEl = document.getElementById('canvas');
        const vpEl  = document.getElementById('viewport');
        const clone = svgEl.cloneNode(true);
        const vpClone = clone.querySelector('#viewport');
        if(vpClone && vpEl){ vpClone.setAttribute('transform', vpEl.getAttribute('transform')); }
        return clone;
      }

      // Inicialización
      update();
      bindInteractions();
      apply(); // evita parpadeos y mantiene flechas si los nodos están visibles
      document.querySelectorAll('.arrow').forEach(showEdgeIfNodesVisible);
    })();
    </script>
    </body></html>"""

    html = (template
            .replace("[[INITCOL]]", str(INIT_COL_CENTER))
            .replace("[[WIDTH]]", str(width))
            .replace("[[HEIGHT]]", str(height))
            .replace("[[EDGES]]", "".join(edges))
            .replace("[[NODES]]", "".join(nodes))
            .replace("[[HEADERS]]", "".join(headers))
            .replace("[[OPT_INITS]]", opt_inits)
            .replace("[[OPT_DEFS]]", opt_defs)
            .replace("[[MODEL_NAME]]", esc(model_name))
           )

    out.write_text(html, encoding="utf-8")
    print("")
    print(f"✓ HTML generado: {out}")
    print("")
    print("Para visualizar:")
    print(f"  1. Abre {out} en un navegador web")
    print("  2. Usa los filtros del panel derecho para explorar el modelo")
    print("  3. Haz clic en iniciadores o defensas para resaltar conexiones")
    print("  4. Zoom: rueda del ratón o botones +/- ")
    print("  5. Pan: arrastra el canvas (Shift para pan rápido)")
    print("")
    print("¡Listo!")

# Execute the script with error handling
if __name__ == "__main__":
    safe_execute()


